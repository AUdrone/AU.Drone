
#include "XBee.h"
 
//#include "../include/"

int XBee::droneNumber;
int XBee::sequenceNumber;

struct __attribute__ ((__packed__)) NavdataToCommRelay {
	uint8_t batteryPower;
	uint32_t altitude;
//	float vy;
//	float vx;
//	float vz;	
};

/**
* XBee::setup()
* sets up xbee port and sends a connect message to the GCS
**/
void XBee::setup()
{
	
	std::cout << "entered setup plox";
	fd = -1;
	noId = true;
	port = "/dev/ttyUSB0";
	baud = 57600;
	flying = false;
	sequenceNumber = 0;
	//mostRecentCommand = 8;

	std::cout << "plz";

	navigator.setup();
	std::cout << "1";
	pi.setup();
	std::cout << "2";
	openPort();
	std::cout << "3";
	setupPort();
	std::cout << "4";
	//sendConnect();
	std::cout << "5";

	run(); //comment me out and uncomment testrun for testing purposes
	//testRun();
}

// testing function, used for testing and demonstration purposes
void XBee::testRun()
{
	struct timeval start, end, diff;
	float dt = 0.0;
	float x = 0.0;
	float y = 0.0;
	float heading = 0.0;

	navDataTime = time(NULL);
	mostRecentCommand = 8;
	while(1)
	{	
		gettimeofday(&start, NULL);
		navdata_demo theNavData = pi.getNavData();
		gettimeofday(&end, NULL);

//
//

		timersub(&end, &start, &diff);

		dt = (float)diff.tv_usec / 1000000.0;
		x = x + dt * (theNavData.vx * cos(theNavData.phi) - theNavData.vy * sin(theNavData.phi));
		
		y = y + dt * (theNavData.vx * sin(theNavData.phi) + theNavData.vy * cos(theNavData.phi));

		std::cout << "vx: " << x << " vy: " << y << " alt: " << theNavData.altitude << "\r\n";
		if(difftime(time(NULL),navDataTime) > .5)
		{
			sendNavData(theNavData);
			navDataTime = time(NULL);
		}
		if (difftime(time(NULL), safeTakeoffTime)  > 100000)
			cout << " " << endl;
		else if(difftime(time(NULL),safeTakeoffTime) > 14.5)
		{
			cout << "HOVER PLEASE STOP MOVING" << endl;
			mostRecentCommand = 5; // hover
		}
		else if(difftime(time(NULL),safeTakeoffTime) > 9.5)
		{
			cout << "ATTACK!" << endl;
			mostRecentCommand = 2;
		}
		else if (difftime(time(NULL),safeTakeoffTime) > 5.5)
		{
			mostRecentCommand  = 5;
			cout << "STABILIZING" << endl;
		}
		cout << "TIME SINCE TAKEOFF: " << difftime(time(NULL),safeTakeoffTime) << endl;
		xBeeResponseLoop();
		sendCommand();
	}
}

// the run loop! 
// starts out by starting the navdata timer
// the loop begins
// the pi gets nav data from the drone
// after the pi hasnt seen navdata to the gcs for 1 second, it sends nav data
// if the drone has had sufficient amount of time to stabilize after takeoff, it calls navigation
// navigation gives the drone an action/direction
// the pi then checks the xbee port to see if a message is there, acting on it if there it is
// it then sends command through udp to the drone
// repeat
void XBee::run()
{
//	navDataTime = time(NULL);
//	bool ignoreMe = false;
	struct timeval start, end, diff;
	float dt = 0.0;
	float x = 0.0;
	float y = 0.0;
	float heading = 0.0;

	int bytesWritten;
	char* buf2;
	char tempMsg = 't';
	buf2 = &tempMsg;
	while(1)
	{
		
		gettimeofday(&start, NULL);
		//send theNavData back over xbee. theNavData is unpacked struct
		navdata_demo theNavData = pi.getNavData();
		gettimeofday(&end, NULL);
		timersub(&end, &start, &diff);

//

		NavdataToCommRelay nav;
		nav.batteryPower = theNavData.vbat_flying_percentage; 
		nav.altitude = theNavData.altitude;
//		nav.vy = theNavData.vy;
//		nav.vx = theNavData.vx;
//		nav.vz = theNavData.vz;
		
		//probabalistically send the nav data
		//in this instance, 1 out of 10 messages
		//will be sent



//

		dt = (float)diff.tv_usec / 1000000.0;
		x = x + dt * (theNavData.vx * cos(theNavData.phi) - theNavData.vy * sin(theNavData.phi));
		
		y = y + dt * (theNavData.vx * sin(theNavData.phi) + theNavData.vy * cos(theNavData.phi));

//		std::cout << "vx: " << theNavData.vx << " vy: " << theNavData.vy << " alt: " << theNavData.altitude << "\r\n";
	
		if(difftime(time(NULL),navDataTime) > 1)
		{
			//sendNavData(theNavData);
			std::cout << theNavData.phi << std::endl;
			bytesWritten = write(fd, (char*)&nav , sizeof(nav));
			std::cout << "Sent " << bytesWritten << std::endl;
			std::cout << "Battery Power: " << int(nav.batteryPower) << endl;
			std::cout << "Altitude: " << nav.altitude << endl;

			navDataTime = time(NULL);
		}

		if (flying && difftime(time(NULL), safeTakeoffTime) > 7.5) {
			//mostRecentCommand = navigator.navigate(theNavData.vx, theNavData.vy, (int)theNavData.psi);

		}
		xBeeResponseLoop();
	//	usleep(990000);
	//	mostRecentCommand = DRONE_HOVER;	
	//	sendCommand();
	}
}

/**
* XBee::sendAck(int seqNum)
* Input: sequence number
* Pack an "acket", send it, increment seq number
**/
void XBee::sendAck(int seqNum)
{
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];

	mavlink_msg_ack_pack(0, 0,&msg, XBee::droneNumber,seqNum,mapNumber);

	uint16_t len = mavlink_msg_to_send_buffer(buf,&msg);
	int bytesWritten = write(fd, buf, len);
	if (bytesWritten == -1) {
		throw XBEEWRITE_FAIL;
	} else if (bytesWritten != len) {
		throw XBEEINCOMPLETEWRITE_FAIL;
	}
		
}

/**
* XBee::sendConnect()
* Called from setup()
* Packs a connect packet and sends it
* Sequence number incremented, but should it be?
**/
void XBee::sendConnect()
{
	mavlink_message_t connectmsg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];

	mavlink_msg_connect_pack(0,0,&connectmsg,0);
	uint8_t len = mavlink_msg_to_send_buffer(buf,&connectmsg);
	int bytesWritten = write(fd, buf, len);

	std::cout << "sent message";

	//how do we handle fails?

	if (bytesWritten == -1) {
		throw XBEEWRITE_FAIL;
	} else if (bytesWritten != len) {
		throw XBEEINCOMPLETEWRITE_FAIL;
	}
    incrementSequenceNumber();
}

/**
* XBee::setupPort()
* sets up the port for XBee
* ask JT if you have any questions
**/
bool XBee::setupPort() {
	struct termios config;
	
	if (!isatty(fd)) {
		throw XBEENOTSERIAL_FAIL;
	}

	if (tcgetattr(fd, &config) > 0) {
		throw XBEEGETCONFIG_FAIL;
	}

	config.c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK
			| ISTRIP | IXON);
	config.c_oflag = 0;
	config.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	config.c_cflag &= ~(CSIZE | PARENB);
	config.c_cflag |= CS8;
	config.c_cc[VMIN] = 0;
	config.c_cc[VTIME] = .01;

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

	if (tcsetattr(fd, TCSAFLUSH, &config) < 0) {
		throw XBEESETCONFIG_FAIL;
	}
	return true;
}

/**
* XBee::openPort()
* opens the port
**/
void XBee::openPort() {
	fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		std::cout << "bad";
		throw INVALID_FD;
	} else {
		std::cout << "good";
		fcntl(fd, F_SETFL, 0);
	}
}

/**
* XBee::shutdown()
* public, call to shutdown Xbee
**/
void XBee::shutdown() {
	closePort();
}

/**
* XBee::closePort()
* called by shutdown
**/
void XBee::closePort() {
	if (close(fd) == -1 ) {
		throw XBEECLOSE_FAIL;
	}
}

/**
* XBee::xBeeResponseLoop
* looks for xbee messages (mavlink) and handles them
**/
void XBee::xBeeResponseLoop()
{
//	mavlink_message_t msg;
//	mavlink_status_t status;
		//get the navdata and send it
		
		uint8_t c, len;
		len = read(fd, &c, sizeof(uint8_t)); //read from XBee
		if (len > 0) {

		std::cout << "gott " << c << "\n";

		switch(c) {

			case 't': 
			{
				mostRecentCommand = TAKEOFF;
				flying = true;
				sendCommand();
				break;
			}

			case 'l':
			{
				mostRecentCommand = LAND;
				sendCommand();
				break;
			}

			case 'w':
			{
				mostRecentCommand = FORWARD;
				sendCommand();
			//	usleep(1000000);
			//	mostRecentCommand = HOVER;
			//	sendCommand();
				break;
                	 }
			

			case 'a':
			{
				mostRecentCommand = LEFT;
				sendCommand();
				break;
			}

			case 's':
			{
				mostRecentCommand = BACK;
				sendCommand();
				break;
			}

			case 'd':
			{
				mostRecentCommand = RIGHT;
				sendCommand();
				break;
			}

			case 'h':
			{
				mostRecentCommand = DRONE_HOVER;
				sendCommand();
				break;
			}

			case 'g':
			{
				mostRecentCommand = UP;
				sendCommand();
				break;
			}

			case 'b':
			{
				mostRecentCommand = DOWN;
				sendCommand();
				break;
			}

			case 'v':
			{
				mostRecentCommand = ROTATE;
				sendCommand();
				break;
			}

			case 'n':
			{
				mostRecentCommand = OROTATE;
				sendCommand();
				break;
			}

			case 'k':
			{
				mostRecentCommand = ENDEMERGENCY;
				sendCommand();
				break;
			}

			case 'c':
			{
				mostRecentCommand = SWITCHCAMERA;
				sendCommand();
				break;
			}

			default:
				std::cout << "got " << c  << endl;

			}
		}
	
			else {
		//	std::cout << "got nothing" << endl;
			//mostRecentCommand = DRONE_HOVER;
//			sendCommand();
		}

	/*	if(mavlink_parse_char(MAVLINK_COMM_0,c,&msg,&status)) //parse the packet
		{

			switch(msg.msgid)
			{
				case MAVLINK_MSG_ID_ACK: //If it's an ack
				{
					mavlink_ack_t ack;
					mavlink_msg_ack_decode(&msg,&ack); 


					if (noId) { // if it is an initial connect message
						droneNumber = ack.drone_number;
						mapNumber = ack.map_number;
						noId = false;
					}
                    			sequenceNumber = ack.seq_number;



					break;
				}
				case MAVLINK_MSG_ID_CMDMSG: //If it's a command message (takeoff,land, etc.)
				{
					mavlink_cmdmsg_t cmdmsg;
					mavlink_msg_cmdmsg_decode(&msg,&cmdmsg);
					if (cmdmsg.drone_number == droneNumber) { 
						sendAck(cmdmsg.seq_number);
                        mostRecentCommand = cmdmsg.command_id;
                        if(cmdmsg.command_id == 1 || cmdmsg.command_id == 7)
                        {
                        	flying = true;
                        	safeTakeoffTime = time(NULL);
                        }
                        std::cout << "Got command with number: " << mostRecentCommand << std::endl;
					}



					break;
				}
				default:
					std::cout << "Nope\n";
					break;
			}*/
        
		

	
}



void XBee::sendCommand()
{	//the following code is used for testing rotation, may not work directly uncommented
	/*if(mostRecentCommand == TURN_LEFT || mostRecentCommand == TURN_RIGHT ||
		mostRecentCommand == CORRECT_LEFT || mostRecentCommand || CORRECT_RIGHT)
	{
		navdata_demo nd;
		if (originalHeading = -1000)
			originalHeading = nd.psi;
		if(mostRecentCommand == TURN_RIGHT)
		{
			int desiredHeading = ((int)nd.psi + 90) % 360;
			
				nd = pi.getNavData();
				if(!(nd.psi < desiredHeading + 2 && nd.psi > desiredHeading - 2))
					mostRecentCommand = 6;
				else
				{
					mostRecentCommand = HOVER;
					originalHeading = -1000;
				}
				cout << "desired heading: " << desiredHeading << endl;
		}
		else if(mostRecentCommand == TURN_LEFT)
		{
			int desiredHeading = ((int)nd.psi - 90) % 360;
			
				nd = pi.getNavData();
				if(!(nd.psi < desiredHeading + 2 && nd.psi > desiredHeading - 2))
					mostRecentCommand = 6;
				else
				{
					mostRecentCommand = HOVER;
					originalHeading = -1000;
				}
			cout << "desired heading: " << desiredHeading << endl;	
		}
		else if(mostRecentCommand == CORRECT_LEFT)
		{
			int desiredHeading = ((int)nd.psi + 10) % 360;
			
				nd = pi.getNavData();
				if(!(nd.psi < desiredHeading + 2 && nd.psi > desiredHeading - 2))
					mostRecentCommand = 6;
				else
				{
					mostRecentCommand = HOVER;
					originalHeading = -1000;
				}
			cout << "desired heading: " << desiredHeading << endl;
		}
		else if(mostRecentCommand == CORRECT_RIGHT)
		{
			int desiredHeading = ((int)nd.psi + 10) % 360;
			
				nd = pi.getNavData();
				if(!(nd.psi < desiredHeading + 2 && nd.psi > desiredHeading - 2))
					mostRecentCommand = 6;
				else
				{
					mostRecentCommand = HOVER;
					originalHeading = -1000;
				}
			cout << "desired heading: " << desiredHeading << endl;
		}
	}*/
	pi.sendCommand(mostRecentCommand);
}

//sends navdata to the gcs
void XBee::sendNavData(navdata_demo theNavData)
{
	mavlink_message_t navDataMessage;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];

	//pack our special new navdata packet here


	mavlink_msg_navdata_pack(0, 0, &navDataMessage, sequenceNumber, droneNumber, 0,
		theNavData.vx, theNavData.vy, theNavData.altitude, theNavData.psi, theNavData.vbat_flying_percentage);
	uint8_t len = mavlink_msg_to_send_buffer(buf,&navDataMessage);
	int bytesWritten = write(fd, buf, len);
	//how do we handle fails?

	if (bytesWritten == -1) {
		throw XBEEWRITE_FAIL;
	} else if (bytesWritten != len) {
		throw XBEEINCOMPLETEWRITE_FAIL;
	}
    incrementSequenceNumber();
}

//increments sequence number
void XBee::incrementSequenceNumber()
{
    XBee::sequenceNumber = (XBee::sequenceNumber++)%255;
}

// jth added
void XBee::readMsg(char *buffer, int &bytesRead) {
    bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) {
        throw XBEEREAD_FAIL;
    }
}
