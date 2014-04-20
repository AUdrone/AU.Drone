#include "xbee.h"

XBee::XBee()
{
}

XBee::~XBee() {
    shutdown();
}

/*
 * This function read data into the buffer.
 * It sets bytesRead to the number of bytes read.
 *
 */
void XBee::readMsg(char *buffer, int &bytesRead) {
    bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        throw XBEEREAD_FAIL;
    }
}

void XBee::readMsg2(NavdataFromPi &buffer, int &bytesRead) {
	//std::cout << "before read " << buffer.altitude;
	char buff[1000];
	buffer.batteryPower = -1;
	buffer.altitude = -1;
	bytesRead = read(fd, (char *)&buffer, sizeof(buffer));
	std::cout << "bytes read: " << bytesRead << std::endl;
	buff[bytesRead] = '\0';
//	for (int i = 0; i < bytesRead; i++) {
//		std::cout << std::hex << buff[i];
//	}
	std::cout << (int)buffer.batteryPower << " " << buffer.altitude;
	std::cout << std::endl << std::endl;
	//std::cout << "after read " << buffer.altitude;
	if(bytesRead == -1) {
	std::cout << "read error" << std::endl;
	}
}

/*
 * This function sends message over XBee using mavlink headers
 *
 */
void XBee::sendMsg(const mavlink_message_t message) {
    u_int8_t buffer[MAVLINK_MAX_PACKET_LEN];
    int len = mavlink_msg_to_send_buffer(buffer, &message);
    int bytesWritten = write(fd, (char *)buffer, len);
    tcflush(fd, TCOFLUSH);
    if (bytesWritten == -1) {
        throw XBEEWRITE_FAIL;
    } else if (bytesWritten != len) {
        throw XBEEINCOMPLETEWRITE_FAIL;
    }
}

void XBee::sendMsg2(char message){
	std::cout << "trying to send " << message << "\n";
	char *buf = &message;
	int bytesWritten = write(fd, buf, 1); 
	std::cout << "sent" << "\n";
	//tcflush(fd, TCOFLUSH);
	 if (bytesWritten == -1) {
        throw XBEEWRITE_FAIL;
    }// else if (bytesWritten != len) {
       // throw XBEEINCOMPLETEWRITE_FAIL;
   // }

}

/*
 * This function initializes the serial port for XBee
 *
 */
void XBee::setup() {
    fd = -1;
    baud = 57600;
    try {
        openPort();
        setupPort();
    } catch (int i) {
        if (handleError(i) == FATAL_ERROR) {
            throw FATAL_ERROR;
        }
    }
}

/*
 * This function searches for the file path of the XBee serial port
 *
 */
void XBee::findPort() {
    //source http://www.signal11.us/oss/udev/

    struct udev *udev;
    struct udev_enumerate *enumerate;
    struct udev_list_entry *devices, *dev_list_entry;
    struct udev_device *dev;

    udev = udev_new();
    if (!udev) {
        throw XBEEUDEVCREATE_FAIL;
    }

    enumerate = udev_enumerate_new(udev);
    udev_enumerate_add_match_subsystem(enumerate, "tty");
    udev_enumerate_scan_devices(enumerate);
    devices = udev_enumerate_get_list_entry(enumerate);

    udev_list_entry_foreach(dev_list_entry, devices) {
        const char *path;
        path = udev_list_entry_get_name(dev_list_entry);
        dev = udev_device_new_from_syspath(udev, path);
        //store the file path of the serial port before we go deeper and change the path name in search of vendor information
        std::string temp = udev_device_get_devnode(dev);

        dev = udev_device_get_parent_with_subsystem_devtype(dev, "usb", "usb_device");
        if (!dev) {
            break;
        }
        std::string ven;
        std::string prod;
        if (udev_device_get_sysattr_value(dev, "idVendor") != NULL) {
            ven = udev_device_get_sysattr_value(dev, "idVendor");
        }
        if (udev_device_get_sysattr_value(dev, "idProduct") != NULL) {
            prod = udev_device_get_sysattr_value(dev, "idProduct");
        }
        //if the product and vendor id match the XBee
        if ( (!strcmp(ven.c_str(), XBEE_VENDORID)) && (!strcmp(prod.c_str(), XBEE_PRODUCTID))) {
            //store the file path and reref pointers
            port = temp;
            udev_device_unref(dev);
            udev_enumerate_unref(enumerate);
            udev_unref(udev);
            return;
        }
        udev_device_unref(dev);
    }
    udev_enumerate_unref(enumerate);
    udev_unref(udev);
    throw NOXBEE_FAIL;
}

/*
 * Helper function that gets the file descriptor for the serial port
 *
 */
void XBee::openPort() {
    try {
        findPort();
    } catch (int i) {
        if (handleError(i) == FATAL_ERROR) {
            throw FATAL_ERROR;
        }
    }

    //chmod 666 read/write for user, group, and other but no execution
    if (chmod(port.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IWOTH | S_IROTH | ~S_IXGRP | ~S_IXOTH | ~S_IXUSR) == -1) {
        //throw chmod fail
        throw XBEECHMOD_FAIL;
    }
    //open the port to read and write
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        throw INVALID_FD;
    } else {
        //clear the fd's settings
        fcntl(fd, F_SETFL, 0);
    }
}

/*
 * Helper function that sets the fd's settings.
 *
 */
void XBee::setupPort() {
    struct termios config;

    if (!isatty(fd)) {
            throw XBEENOTSERIAL_FAIL;
        }

        if (tcgetattr(fd, &config) > 0) {
            throw XBEEGETCONFIG_FAIL;
        }

        /*
         * The following block of code sets the fd to noncanonical mode (raw input).
         * Input to the fd's buffer is avaible character by character instead of line by line.
         *
         */

        config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK
                | ISTRIP | IXON);
        config.c_oflag = 0;
        config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
        config.c_cflag &= ~(CSIZE | PARENB);
        config.c_cflag |= CS8;

        //this sets the fd to nonblocking, the minimum amount of chars that have to be read before read() returns
        config.c_cc[VMIN] = 0;
        //this sets the time that read will hang waiting for the next char in deciseconds
        config.c_cc[VTIME] = .01;

        //this switch sets the baud rate
        switch (baud) {
            case 1200:
                if (cfsetispeed(&config, B1200) < 0 ||
                        cfsetospeed(&config, B1200) < 0) {
                    throw XBEEBAUD_FAIL;
                }
                break;
            case 1800:
                cfsetispeed(&config, B1800);
                cfsetospeed(&config, B1800);
                break;
            case 9600:
                cfsetispeed(&config, B9600);
                cfsetospeed(&config, B9600);
                break;
            case 19200:
                cfsetispeed(&config, B19200);
                cfsetospeed(&config, B19200);
                break;
            case 38400:
                if (cfsetispeed(&config, B38400) < 0 ||
                        cfsetospeed(&config, B38400) < 0) {
                    throw XBEEBAUD_FAIL;
                }
                break;
            case 57600:
                if (cfsetispeed(&config, B57600) < 0 ||
                        cfsetospeed(&config, B57600) < 0) {
                    throw XBEEBAUD_FAIL;
                }
                break;
            case 115200:
                if (cfsetispeed(&config, B115200) < 0 ||
                        cfsetospeed(&config, B115200) < 0) {
                    throw XBEEBAUD_FAIL;
                }
                break;
            default:
                if (cfsetispeed(&config, B115200) < 0 ||
                        cfsetospeed(&config, B115200) < 0) {
                    throw XBEEBAUD_FAIL;
                }
                break;
        }

        //apply all the setting to the fd
        if (tcsetattr(fd, TCSAFLUSH, &config) < 0) {
            throw XBEESETCONFIG_FAIL;
        }
}

/*
 * This function shutdown XBee
 *
 */
void XBee::shutdown() {
    closePort();
}

/*
 * Helper function that closes the file descriptor
 *
 */
void XBee::closePort() {
    if (close(fd) == -1) {
        throw XBEECLOSE_FAIL;
    }
}

/*
 * This function is an early attempt at handling and recovering from errors.
 * FATAL_ERRORs should end exection and NONFATAL_ERRORs should try to be resolved
 *
 */
bool XBee::handleError(int i) {
    switch (i) {
            case NOXBEE_FAIL:
                std::cerr << "No XBee was detected\n";
                return FATAL_ERROR;
            case XBEECHMOD_FAIL:
                std::cerr << "XBee failed to chmod the port fd. Did you forget sudo?\n";
                return FATAL_ERROR;
            case INVALID_FD:
                std::cerr << "Inavlid fd\n";
                return FATAL_ERROR;
            case XBEEREAD_FAIL:
                std::cerr << "XBee read fail\n";
                return FATAL_ERROR;
            case XBEEWRITE_FAIL:
                std::cerr << "XBee write fail\n";
                return FATAL_ERROR;
            case XBEENOTSERIAL_FAIL:
                std::cerr << "XBee not a serial port\n";
                return FATAL_ERROR;
            case XBEEGETCONFIG_FAIL:
                std::cerr << "XBee could not get the port configuration settings\n";
                return FATAL_ERROR;
            case XBEESETCONFIG_FAIL:
                std::cerr << "XBee could not set the port configuration settings\n";
                return FATAL_ERROR;
            case XBEEBAUD_FAIL:
                std::cerr << "XBee could not set the baud rate\n";
                return FATAL_ERROR;
            case XBEECLOSE_FAIL:
                std::cerr << "XBee failed to close the port\n";
                return FATAL_ERROR;
                break;
            case XBEEINCOMPLETEWRITE_FAIL:
                std::cerr << "XBee did not write all the data\n";
                return NONFATAL_ERROR;
                break;
            default:
                return FATAL_ERROR;
                break;
        }
}
