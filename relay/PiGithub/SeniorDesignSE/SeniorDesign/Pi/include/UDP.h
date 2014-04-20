/*
*
* UDP.h
* Class for UDP communication with drone
* 
*
*/ 

#ifndef _UDP_H_
#define _UDP_H_

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
#include <iostream>

#define AT_PORT 5556
#define NAVDATA_PORT 5554
#define WIFI_MYKONOS_IP "192.168.1.1"

enum commands {
	TAKEOFF = 1,
	FORWARD,
	LEFT,
	RIGHT,
	DRONE_HOVER,
	ROTATE,
	LAND,
	WATCHDOG,
	CALIBMAGNETO,
	GETNAVDATA,
	BACK,
	UP,
	DOWN,
	OROTATE,
	ENDEMERGENCY,
	SWITCHCAMERA
};

class UDP {
	public:
		void sendCommand(int commandCode);
		void setup();
		bool handleError(int i);
		void reset();
	private:
		void takeoff();
		void forward();
		void back();
		void left();
		void right();
		void rotate();
		void hover();
		void land();
		void up();
		void down();
		void orotate();
		void endemergency();
		void switchcamera();
		void watchDog();
		void calib_magneto();
		void getNavData();
		void navdata_write(char* command);

		int seq;
		int at_socket;
		struct sockaddr_in drone_at;
		struct sockaddr_in drone_nav;
		float forwardPower;
		float rotPower;
		float orotPower;
		float tilt_left;
		float tilt_right;

};

#endif

