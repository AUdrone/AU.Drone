/*
*
* NavData.h
*	Handles nav data input + parsing
*	Reads from socket, makes sense of it
*/

#ifndef _NAVDATA_H_
#define _NAVDATA_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include "ConstantsAndHeaders.h"


#define NAVDATA_PORT         5554
#define NAVDATA_BUFFER_SIZE     2048
#define WIFI_MYKONOS_IP "192.168.1.1"

class NavData 
{
public:
	void setup();
	void navdata_read();
	bool handleError(int i);
	struct navdata_demo getNavDataDemo();
private:
	void parse_navdata();
	void getNextOption(int *index);
	void parse_navdemo(int *index);
	void parse_checksum(int *index);
	
	struct navdata_demo navdemoData;
	struct sockaddr_in pc_addr;
	struct sockaddr_in drone_nav;
	struct sockaddr_in from; 

	int navdata_socket;
	int32_t drone_state;
	int32_t sequenceNumber;
	int32_t visionFlag;

	char navdata_message[NAVDATA_BUFFER_SIZE];


};

#endif
