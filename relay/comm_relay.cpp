#include <iostream> 
#include <stdlib.h> 
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <unistd.h>
#include <list>
#include <pthread.h>
#include <cerrno>
#include <netinet/in.h>
#include "legacy/xbee.h"

#define PORT 4710 
#define BUFSIZE 2048 
#define XBEEPACKET_SLEEP_TIME 1000.0

  
using namespace std;

XBee xb;
char mavlinkBuffer[MAX_BUFF_LEN];

/*
struct __attribute__ ((__packed__)) NavdataFromPi {
	uint8_t batteryPower;
	uint32_t altitude;
	float vy;
	float vx;
	float vz;
};
*/

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
//    comm_relay cr; 
    xb.setup();
//	xb.sendMsg2('t');

//	usleep(9000000);


//Josh's poop
/*
int c;

do {
	c = getchar();

	switch(c) {

	case 't':
	case 'l':
		xb.sendMsg2(c);
		break;
	}
} while (c != 'q');

for(int i = 0; i < 15; i++) {
	xb.sendMsg2('l');
}
*/
//Charizard is the shit

//Robert's stuff 

	
	struct addrinfo hints;
	struct addrinfo *res;
	socklen_t fromlen;
	struct sockaddr_storage addr;
	int bytes_received = 0;
	int navdata_bytes_received = 0;
	
	//flush out the details of the socket
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;
	
	//tell the socket where to send navdata (loopback)
	struct sockaddr_in their_addr;
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(4711);
	inet_pton(AF_INET, "127.0.0.1", &(their_addr.sin_addr));
	memset(their_addr.sin_zero, '\0', sizeof(their_addr.sin_zero));

	//get the address infor for socket
	if(0 != getaddrinfo(NULL, "4710", &hints, &res)) {
		cout << "getaddrinfo ERROR" << endl;
		exit(1);
	}

	//make the socket
	int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	
	//make sure socket is valid
	if (-1 == sockfd) {
		cout << "socket ERROR" << endl;
		exit(1);
	}

	//bind the socket
	if(-1 == bind(sockfd, res->ai_addr, res->ai_addrlen)) {
		cout << "binding ERROR" << endl;
		exit(1);
	}
	
	//set the length of the timeout for the socket
	struct timeval timeout;
	timeout.tv_sec = 1; //seconds
	timeout.tv_usec = 0; //microseconds
	
	//change the socket to allow timeouts
	if(0 > setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout))) {
		cout << "setsockopt ERROR!" << endl;
		exit(1);
	}

	fromlen = sizeof addr;
	char commandBuffer[2];
	int command;	
//	NavdataFromPi navPi;
//	memset(&navPi, 0, sizeof(navPi));
//	char *nav = (char*) malloc(1); //17 is the size of the navdata packets we are receiving
//	memset(&nav, 0, sizeof(nav));
	int xbeeBytesRead = 0;
//	char *nav;
	NavdataFromPi nav;
//	navjoshuat nav[1];
	nav.altitude = 4;
	uint8_t tempBatPower;
	int c;

	/*
	*begin loops to read commands from the website,
	*read navdata from the raspberry pi, and send navdata
	*back to the server.
	*/

	do {
//	cout << "receiving" << endl;
	
	do {	
		xbeeBytesRead = 0;
		bytes_received = 0;
		errno = 0;
	//	bytes_received = recvfrom(sockfd, (char*) &commandBuffer, sizeof(int), 0, (sockaddr*) &addr, &fromlen);
		xb.readMsg2(nav, xbeeBytesRead); //read from the xbee to get navdata
	//	xbeeBytesRead = read(fd, (char*) &nav, sizeof(nav));
		
		//if we received something, send it back to the server
		if(xbeeBytesRead > 0) {
			cout << "Gottttttttttttttttt" << endl;
			cout << "XbeeBytesRead: " << xbeeBytesRead << endl;
			//tempBatPower = (uint8_t) nav[0];
			cout << "Battery Power: " << int(nav.batteryPower) << endl;
			cout << "Altitude: " << uint32_t(nav.altitude) << endl;
			if(-1 == sendto(sockfd, &nav, sizeof(nav), 0, (struct sockaddr *) &their_addr, sizeof(their_addr))) {
				cout << "sendto ERROR" << endl;
				exit(1);
			}

		//	cout << "Batter Power2: " << nav << endl;
		}
		
		//try to get a command from the website. if none is sent, timeout.
		//else process the command
		bytes_received = recvfrom(sockfd, commandBuffer, sizeof commandBuffer, 0, (sockaddr*) &addr, &fromlen);
	} while(-1 == bytes_received); 

	if (bytes_received > 1) {
	command = atoi(commandBuffer);
	}
	else {
		command = (uint8_t) commandBuffer[0];
		command = command - 48;
	}
//	command = ntohl(command);
//	cout << "bytes received " << bytes_received << endl;
//	cout << "received " << command << endl; 
		//c = getchar(); 
	
	if (command == 0) {
		cout << "forward" << endl;
		xb.sendMsg2('w');
	}

	if (command == 1) {
		cout << "backward" << endl;
		xb.sendMsg2('s');
	}
	
	if (command == 2) {
		cout << "left" << endl;
		xb.sendMsg2('a');
	}

	if (command == 3) {
		cout << "right" << endl;
		xb.sendMsg2('d');
	}

	if (command == 4) {
		cout << "up" << endl;
		xb.sendMsg2('g');
	}

	if (command == 5) {
		cout << "down" << endl;
		xb.sendMsg2('b');
	}

	if (command == 6) {
		cout << "rotate left" << endl;
		xb.sendMsg2('v');
	}

	if (command == 7) {
		cout << "rotate right" << endl;
		xb.sendMsg2('n');
	}

	if(command == 8) {
		cout << "IIIIIII just wanna fly" << endl;
		for (int y = 0; y < 2; y++) {
		xb.sendMsg2('t');
		xb.sendMsg2('t');
		}
	}
	
	if(command == 9) {
		cout << "landing" << endl;
		xb.sendMsg2('l');
	}

	if(command == 10) {
		cout << "hovering" << endl;
		xb.sendMsg2('h');
		xb.sendMsg2('h');
	}

	if(command == 15 ) { //emergency mode
		cout << "stop emergency mode" << endl;
		xb.sendMsg2('k');
	}

	if(command == 16) { //switch camera
		cout << "changing camera view" << endl;
		xb.sendMsg2('c');
	}

	} while (commandBuffer[0] != 'b');


//Robert's stuff 



		/*switch(commandBuffer[0]) {

			case '8':
			xb.sendMsg2('t');
			break;
		}*/
//	} while (commandBuffer[0] != '9');
	

/*	for(int i = 0; i < 15; i++) {

	xb.sendMsg2('l');
	} 
*/
//	cr.udpThread();
 
  //  while(1) { 
  //	 cr.read();
  //	}
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
//            comm_relay::sendCommand(c);		

	    //adam test
//	    c.cmdid = TAKEOFF_COMMAND;
	    //c.param1++; 
	//	for( int i=0; i<10000; i++ ) {
	
//	    comm_relay::sendCommand(c);
	//	std::cout << "passed takeoff";
	//	}
	    //comm_relay::sendCommand(c);
//	usleep(1000);
//		std::cout << "passed sleep";
//	    c.cmdid = LAND_COMMAND;
//	    comm_relay::sendCommand(c);

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

//	mavlink_message_t message;
	int takeoff = 0;
	char commandToSend;
	while(1) 
    { 
//        cout<<"Waiting on port: "<<PORT<<"\n"; 
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen); 
 //       cout<<"Received "<<recvlen<<" bytes on port: "<<PORT<<"\n"; 
        if(recvlen > 0) 
        { 
            buf[recvlen] = 0; 
            cout<<"Received message: "<<buf<<"\n";
	cout << "buf[0] = " << buf[0] << "\n";            
		

		//the commandIndex now indicates the postion of the numeric command sent

	    switch(buf[0]) {
		
		case '0':
		commandToSend = 'w'; 
		break;
		
		case '1':
		commandToSend = 's';
		break;
		
		case '2':
		commandToSend = 'a';
		break;

		case '3':
		commandToSend = 'd';
		break;

		case '4': 
		commandToSend = 'h'; /////////////////////////set to hover needs to be ascend
		break;

		case '5':
		commandToSend = 'h'; //////////////////////////set to hover, needs to be descend
		break;

		case '6':
		commandToSend = 'h'; ///////////////////////////set to hover, needs to be rotate left
		break;

		case '7':
		commandToSend = 'h'; /////////////////////////////set to hover, needs to be rotate right
		break;

		case '8':
		commandToSend = 't';
		break;

		case '9':
		commandToSend = 'l';
		break;

		default:
		commandToSend = 'h'; /////////////problem we only have 10 ascii characters to work with, darn
		break;
	    }

//	    mavlink_msg_cmdmsg_pack(0, 0, &message, 0, takeoff++, 0, 0, 0, 0);

		try{
		    xb.sendMsg2(commandToSend);
			std::cout << "tried to send " << commandToSend << "\n";	
                } catch (int i) {
			cout << "fatal error\n";
		    if (xb.handleError(i) == FATAL_ERROR) {
		    throw FATAL_ERROR;
		    }
		}
/*	    if (message.len > 0) {
		try {
		    xb.sendMsg(message);
		    cout << "Message Sent to XBee: " << message.len << endl;
		} catch (int i) {
		    if (xb.handleError(i) == FATAL_ERROR) {
			throw FATAL_ERROR;
		    }
		}
	    }*/
        } 
    }
    close (fd);	 
    return 0;
}
