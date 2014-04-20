#include "XBee.h"

int main()
{

	std::cout << "just work";
	 XBee xB;
	std::cout << "created xB";
	xB.setup();
	std::cout << "after setup";
	

/*	UDP udp;

	udp.setup();

	for (int i = 0;i < 20;i++) {
	udp.sendCommand(1);
	}

	usleep(3000000);

	for (int i = 0; i < 20;i++) {
	udp.sendCommand(7);	
	}*/

	return 0;
}
