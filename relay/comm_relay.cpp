#include <iostream> 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <list>

#include "legacy/xbee.h"

#define PORT 4710 
#define BUFSIZE 2048 
#define XBEEPACKET_SLEEP_TIME 1000.0
  
using namespace std;

XBee xb;
char mavlinkBuffer[MAX_BUFF_LEN];

class comm_relay
{
	public:
		void sendCommand(struct COMMAND c);
		void read();
		void receiveCommand(struct COMMAND c);
		int udpThread();
};

int main(int argc, char **argv) 
{ 
/*    struct sockaddr_in myaddr; // server address 
    struct sockaddr_in remaddr; // remote address 
    socklen_t addrlen = sizeof(remaddr); 
    int recvlen; 
    int fd; 
    unsigned char buf[BUFSIZE]; 
8?  
    /* Create a UDP socket */
/*    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("Cannot create socket.\n"); 
        return 0; 
    } 
*/  
    /* Bind socket to any valid IP addresses and a specific port */
  
/*    memset((char *)&myaddr, 0, sizeof(myaddr)); 
    myaddr.sin_family = AF_INET; 
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    myaddr.sin_port = htons(PORT); 
  
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
    { 
        perror("Could not bind socket. Exiting Now."); 
        return 0; 
    } 
*/ 
    comm_relay cr; 
    xb.setup();
    while(1) { 
	 cr.read();
	}
/*    mavlink_message_t message;
    int takeoff = 0;
*/
    /* Loop infinitely to receive data and send commands */
/*    while(1) 
    { 
        cout<<"Waiting on port: "<<PORT<<"\n"; 
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen); 
        cout<<"Received "<<recvlen<<" bytes on port: "<<PORT<<"\n"; 
        if(recvlen > 0) 
        { 
            buf[recvlen] = 0; 
            cout<<"Received message: "<<buf<<"\n";
          
	    mavlink_msg_cmdmsg_pack(0, 0, &message, 0, takeoff++, 0, 0, 0, 0);
            
	    if (message.len > 0) {
		try {
		    xb.sendMsg(message);
		    cout << "Message Sent to XBee: " << message.len << endl;
		} catch (int i) {
		    if (xb.handleError(i) == FATAL_ERROR) {
			throw FATAL_ERROR;
		    }
		}
	    }
        } 
    } 
  
    close(fd); 
*/
    return 0; 
}

void comm_relay::sendCommand(struct COMMAND c) {
    mavlink_message_t message;
    switch (c.cmdid) {
    case ROTATE_COMMAND:
    case LAND_COMMAND:
	std::cout << "sent land";
    case TAKEOFF_COMMAND:
    case FORWARD_COMMAND:
    case HOVER_COMMAND:
        cerr << "Sending... ";
        mavlink_msg_cmdmsg_pack(0, 0, &message, 0, c.cmdid, c.droneid, c.param1, c.param2, c.param3);
        cerr << "packet: command packed\n";
        break;
    //for ack commands sequence number is stored in param1 and map number in param2
    case ACK_COMMAND:
        mavlink_msg_ack_pack(0, 0, &message, c.droneid, (int)c.param1, (int)c.param2);
        std::cout << c.droneid << " droneId ACK n\n";
        break;
    default:
        message.len = 0;
        break;
    }

    if (message.len != 0) {
        try {
            xb.sendMsg(message);
		cerr << "xbSendSuccess\n";
        } catch (int i) {
            if (xb.handleError(i) == FATAL_ERROR) {
                throw FATAL_ERROR;
            }
        }
    }
    cerr << "Command sent\n";
} 

void comm_relay::read() {
    int bytesRead;

    //send the buffer to XBee
    xb.readMsg(mavlinkBuffer, bytesRead); //might need this in try catch


    //if we have a read any data
    if (bytesRead > 0) {
	//std::cout << "Got something";
        u_int8_t messageReceived = false;
        mavlink_message_t message;
        mavlink_status_t status;
        //parse every byte read into mavlink internal buffer
        //data sent to the internal buffer is remembered by mavlink
        for (int i = 0; i < bytesRead; i++) {
            messageReceived = mavlink_parse_char(MAVLINK_COMM_1, mavlinkBuffer[i], &message, &status);
            //if we have a full message
	   //std::cout << "Message ID: " << message.msgid << std::endl;
            if (messageReceived) {
		//std::cout << "Plz yolo";

                switch (message.msgid) {
                //for connection commands, sequence number is stored in param1
                case MAVLINK_MSG_ID_CONNECT:
                    mavlink_connect_t conn;
                    mavlink_msg_connect_decode(&message, &conn);
                    struct COMMAND c;
                    c.cmdid = CONNECT_COMMAND;
                    c.param1 = (float)conn.seq_number;
        //          emit xBCommandReceived(c);
		    std::cout << "Got connect msg\n";
		    comm_relay::receiveCommand(c);
                    break;
		case MAVLINK_MSG_ID_ACK:
	        	mavlink_ack_t ack;
			mavlink_msg_ack_decode(&message, &ack);
			std::cout << "plz just yolo once";
			break;
		  default:
                    break;
                }
            }
        }
    }
    /*for( int i =0; i < bytesRead; i++ )
	std::cout << mavlinkBuffer[i];*/

}


void comm_relay::receiveCommand(struct COMMAND c) {
    switch(c.cmdid) {
    case CONNECT_COMMAND: //the only time we get an ack command is when a drone is connecting
            c.cmdid = ACK_COMMAND;
            c.droneid = 1;
            cerr << "New drone id: " << c.droneid << std::endl;
            //c.param1 already has seqNum stored in it
            c.param2 = c.droneid; //hard coding map number for right now
            //create necessary threads here
            //new vid thread
   //       connect(this, SIGNAL(gotVidStream(VIDSTREAM)), &vidThreads[c.droneid], SLOT(receiveVidStream(VIDSTREAM)), Qt::QueuedConnection);
  //        vidThreads[c.droneid].start();
            cerr << "Setting drone id to: " << c.droneid << std::endl;
   //       emit sendCommand(c);
            comm_relay::sendCommand(c);		

	    //adam test
	    c.cmdid = TAKEOFF_COMMAND;
	    //c.param1++; 
	//	for( int i=0; i<10000; i++ ) {
	
	    comm_relay::sendCommand(c);
	//	std::cout << "passed takeoff";
	//	}
	    //comm_relay::sendCommand(c);
	//	sleep(2);
		std::cout << "passed sleep";
	    c.cmdid = LAND_COMMAND;
	    comm_relay::sendCommand(c);

        break;
    default:
        break;
    }
}

int comm_relay::udpThread(){

    struct sockaddr_in myaddr; // server address 
    struct sockaddr_in remaddr; // remote address 
    socklen_t addrlen = sizeof(remaddr); 
    int recvlen; 
    int fd; 
    unsigned char buf[BUFSIZE]; 
  
    /* Create a UDP socket */
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    { 
        perror("Cannot create socket.\n"); 
        return 0; 
    } 
  
    /* Bind socket to any valid IP addresses and a specific port */
  
    memset((char *)&myaddr, 0, sizeof(myaddr)); 
    myaddr.sin_family = AF_INET; 
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    myaddr.sin_port = htons(PORT); 
  
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) 
    { 
        perror("Could not bind socket. Exiting Now."); 
        return 0; 
    } 

	mavlink_message_t message;
	int takeoff = 0;

	while(1) 
    { 
        cout<<"Waiting on port: "<<PORT<<"\n"; 
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen); 
        cout<<"Received "<<recvlen<<" bytes on port: "<<PORT<<"\n"; 
        if(recvlen > 0) 
        { 
            buf[recvlen] = 0; 
            cout<<"Received message: "<<buf<<"\n";
          
	    mavlink_msg_cmdmsg_pack(0, 0, &message, 0, takeoff++, 0, 0, 0, 0);
            
	    if (message.len > 0) {
		try {
		    xb.sendMsg(message);
		    cout << "Message Sent to XBee: " << message.len << endl;
		} catch (int i) {
		    if (xb.handleError(i) == FATAL_ERROR) {
			throw FATAL_ERROR;
		    }
		}
	    }
        } 
    }
    close (fd);	 
    return 0;
}
