#ifndef CONSTANTSANDHEADERS_H
#define CONSTANTSANDHEADERS_H

#define FATAL_ERROR 1
#define NONFATAL_ERROR 0

#define SOCKET_FAIL  1
#define BIND_FAIL  2
#define SENDTO_FAIL  3
#define RECVFROM_FAIL  4
#define INVALID_FD  5
#define XBEEREAD_FAIL  6
#define XBEEWRITE_FAIL  7
#define XBEEINCOMPLETEWRITE_FAIL  8
#define XBEENOTSERIAL_FAIL  9
#define XBEEGETCONFIG_FAIL 10
#define XBEEBAUD_FAIL 11
#define XBEESETCONFIG_FAIL 12
#define XBEECLOSE_FAIL 13
#define NOXBEE_FAIL 14
#define XBEECHMOD_FAIL 15
#define XBEEUDEVCREATE_FAIL 16

#define XBEETIMEOUT_TIME 2.0
#define XBEETIMEOUT_THRESHOLD 5

#define MAX_DRONES 4
#define MAX_BUFF_LEN 1024

#define CONNECT_COMMAND 1
#define ACK_COMMAND 2
#define TAKEOFF_COMMAND 3
#define LAND_COMMAND 4

#define MOVE_FORWARD 2
#define CORRECT_RIGHT 14
#define CORRECT_LEFT 13
#define TURN_LEFT 15
#define TURN_RIGHT 16
#define HOVER 5
#define WATCH_DOG 8

#define ALL_CLEAR 1
#define OBSTACLE_ON_LEFT 2
#define OBSTACLE_ON_RIGHT 3
#define OBSTACLE_IN_FRONT 4

#include <stdint.h>

struct COMMAND {
    int cmdid;
    int droneid;
    float param1;
    float param2;
    float param3;
};

struct navdata_demo {
	int32_t ctrl_state;
	uint8_t vbat_flying_percentage; //battery
	float theta; //pitch
	float psi; //heading
	float phi; //roll
	uint32_t altitude; //in cm
	float vy;
	float vx;
	float vz;
	uint8_t num_frames;
};

#endif // CONSTANTSANDHEADERS_H
