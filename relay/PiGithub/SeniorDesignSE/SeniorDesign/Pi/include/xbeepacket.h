#ifndef XBEEPACKET_H
#define XBEEPACKET_H

#include <algorithm>
#include <cstring>
#include <ctime>
#include <iostream>
#include <list>
#include <map>

#include <ConstantsAndHeaders.h>
#include <mavlink/common/mavlink.h>
#include <XBee.h>

#define INIT_SEQNUM 1
#define XBEEPACKET_SLEEP_TIME 1000.0

/*
 * This struct is represents an enty into the ack db.
 * If a message does not receive an ack, the message is resent with the same sequence number
 *
 */
struct ACK_ENTRY {
    time_t start;
    int timeouts;
    struct COMMAND command;
    int seqNum;
};

class XBeePacket
{
   
public:
    explicit XBeePacket();
    void setup();
    void xBSendSlot(struct COMMAND c);
    void xBCommandReceived(struct COMMAND c);

protected:
	void run();

private:
   // void addEntry(const struct COMMAND c);
   // void checkEntry(int droneId, int seqNumIn);
    void read();
    void sendCommand(struct COMMAND c);
   // void resendCommand(struct ACK_ENTRY &a);
   // void sendACK(int droneId, int seqNumIn, int mapNumIn);
   // QMutex ackMux;
    XBee xb;
    //an array of maps, 1 map for each drone id, that stores ack entries based on their sequence number
    std::map<int, struct ACK_ENTRY> pendingAcks[MAX_DRONES];
    //list of drones that are disconnected
    std::list<int>disconnectedDrones;
    //buffer that XBee writes to
    char mavlinkBuffer[MAX_BUFF_LEN];
    int seqNum;
};

#endif // XBEEPACKET_H
