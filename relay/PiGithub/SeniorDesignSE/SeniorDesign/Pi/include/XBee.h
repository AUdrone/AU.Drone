/**
* XBee (Pi side)
* 
* Communication protocol for Pi
* Has access to the Pi via Pi*
* Calls Pi functions go get data, builds into packets, and sends
**/

#ifndef _XBEE_H_
#define _XBEE_H_

#include "Pi.h"
#include "mavlink/common/mavlink.h"
#include "ConstantsAndHeaders.h"
#include "Navigation.h"
#include <errno.h>
#include <cmath>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include "fcntl.h"
#include "sys/stat.h"
#include <time.h>
#include <vector>
#include <iterator>

/*
#define MAVLINK_MSG_ACK 151
#define MAVLINK_MSG_CMDMSG 152
#define MAVLINK_MSG_CONNECT 150
*/

#define MAXDATASIZE 2048

using std::cout;
using std::string;
using std::iterator;


class XBee 
{
public:
	void setup(); //initalizes Pi communication, calls setupPort(), openPort(), and connect()
	void readMsg(char *buffer, int &bytesRead);

	void shutdown(); //shuts down xbee port
private:
	Pi pi;
	Navigation navigator;
	static int droneNumber;
	int mapNumber;
	string port;
	int baud;
	int fd;
	bool flying;
	static int sequenceNumber;
	float originalHeading;
	int mostRecentCommand;
	time_t navDataTime;
	time_t safeTakeoffTime;
	bool setupPort(); //sets up port for Xbee
	void openPort(); //opens the port
	void closePort(); //closes xbee port
	void sendAck(int seqNum); //passes the packed packet to sendAck, which sends it
	void sendConnect(); //sends connect message to GCS
	void sendCommand();
    void run(); //run loop, checks for packets, reads, handles them
	void xBeeResponseLoop();
	void navDataLoop();
	void sendNavData(navdata_demo theNavData);
	void sendVideo();
	void vidSetup();
    void incrementSequenceNumber();
	void *get_in_addr(struct sockaddr *sa);
	void testRun();
	
	int status, sockfd, numBytes;
	char buf[MAXDATASIZE];
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servinfo, *p;
	bool noId;
};

#endif
