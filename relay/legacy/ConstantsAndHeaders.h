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

#define TAKEOFF_COMMAND 1
#define FORWARD_COMMAND 2
#define LEFT_COMMAND 3
#define RIGHT_COMMAND 4
#define HOVER_COMMAND 5
#define ROTATE_COMMAND 6
#define LAND_COMMAND 7
#define WATCHDOG_COMMAND 8
#define CALIBMAGNETO_COMMAND 9
#define GETNAVDATA_COMMAND 10
#define BACK_COMMAND 11
#define CONNECT_COMMAND 12
#define ACK_COMMAND 13
#define NAVDATA_COMMAND 14
#define VIDSTREAM_COMMAND 15

struct COMMAND {
    int cmdid;
    int droneid;
    float param1;
    float param2;
    float param3;
};

struct NAVDATA {
    int seqNum;
    int droneId;
    float vx;
    float vy;
    float altitude;
    float heading;
    int battery;
    int dronestate;
};

struct VIDSTREAM {
    int seqNum;
    int droneId;
    char buff[128];
};

#endif // CONSTANTSANDHEADERS_H
