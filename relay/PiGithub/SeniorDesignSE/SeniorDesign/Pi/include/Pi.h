#ifndef PI
#define PI

#include "UDP.h"
#include "NavData.h"

class Pi
{
public:
	void setup();
	void sendCommand(int commandNumber);
	navdata_demo getNavData();
	void blastCommand(int commandNumber);
private:
	UDP udp;
	NavData navData;
};

struct command {
    int id;
    double param1;
    double param2;
    double param3;
};


#endif