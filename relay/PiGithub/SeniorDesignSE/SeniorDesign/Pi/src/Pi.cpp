#include  "Pi.h"



void Pi::setup()
{
	navData.setup();
	udp.setup();
}

void Pi::sendCommand(int commandCode)
{
	udp.sendCommand(commandCode);
}

navdata_demo Pi::getNavData()
{
	udp.sendCommand(GETNAVDATA);
	navData.navdata_read();
	navdata_demo data = navData.getNavDataDemo();
	if (data.ctrl_state & 1 << 30)
		udp.reset();
	return data;
}

void Pi::blastCommand(int c)
{
	for(int i = 0; i < 10; i++)
	{
		sendCommand(c);
	}
}
