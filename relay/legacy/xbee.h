#ifndef XBEE_H
#define XBEE_H

#include <string>
#include <iostream>
#include <libudev.h>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <ctime>
#ifdef __linux
    #include <sys/ioctl.h>
    #include <sys/stat.h>
#endif

#include "ConstantsAndHeaders.h"
#include "mavlink/common/mavlink.h"

//vendor and product id for XBee s1
#define XBEE_VENDORID "0403"
#define XBEE_PRODUCTID "6001"

class XBee
{
public:
    XBee();
    ~XBee();
    void setup();
    void readMsg(char *buffer, int &bytesRead);
    void sendMsg(const mavlink_message_t message);
    void sendMsg2(char command);
    void shutdown();
    bool handleError(int i);

private:
    void findPort();
    void setupPort();
    void openPort();
    void closePort();

    int baud;
    std::string port;
    int fd;
};

#endif // XBEE_H
