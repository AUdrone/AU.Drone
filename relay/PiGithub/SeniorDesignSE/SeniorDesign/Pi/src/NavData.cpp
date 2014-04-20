#include "NavData.h"

void NavData::setup()
{
	int32_t one = 1;
	navdata_socket =  -1;

	if((navdata_socket = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
        	printf ("navdata_socket: %s\n", strerror(errno));
        	throw SOCKET_FAIL;
    	}

   	pc_addr.sin_family = AF_INET;
 	pc_addr.sin_addr.s_addr = htonl(INADDR_ANY);
 	pc_addr.sin_port = htons(NAVDATA_PORT);

	drone_nav.sin_family = AF_INET;
	drone_nav.sin_addr.s_addr = inet_addr(WIFI_MYKONOS_IP);
   	drone_nav.sin_port = htons(NAVDATA_PORT);

 	if(bind( navdata_socket, (struct sockaddr *)&pc_addr, sizeof(pc_addr)) < 0){
        	throw BIND_FAIL;         	
    	}

    	//set unicast mode on
    	int error = sendto(navdata_socket, &one, 4, 0, 
			(struct sockaddr *)&drone_nav, sizeof(drone_nav));
	if (error == -1) {
		throw SENDTO_FAIL;
	}
}

void NavData::navdata_read()
{
	// blocking call waiting for navdata
	// if no navdata is received, program infintely waits

	int size, l;
 	size = 0;
 	size = recvfrom ( navdata_socket, &navdata_message[0], NAVDATA_BUFFER_SIZE, 0x0, (struct sockaddr *)&from, (socklen_t *)&l);

	if (size == -1) {
		throw RECVFROM_FAIL;
	}
 	parse_navdata();
}

void NavData::parse_navdata()
{
	//printf("parse navdata\n");
 	//uint8_t navdata_message[index] = navdata_message;
	int index = 0;
 	int32_t header;
 	header = navdata_message[index];
 	index += 1;
 	header |= (navdata_message[index] << 8);
 	index += 1;
 	header |= (navdata_message[index] << 16);
 	index += 1;
 	header |= (navdata_message[index] << 24);
 	index += 1;
 	if (header != 0x55667788) {
 		return;
 	}
 	drone_state = navdata_message[index];
 	index += 1;
 	drone_state |= (navdata_message[index] << 8);
 	index += 1;
 	drone_state |= (navdata_message[index] << 16);
 	index += 1;
 	drone_state |= (navdata_message[index] << 24);
 	index += 1;
 	//printf("drone_state: %x\n", drone_state);

 	sequenceNumber = navdata_message[index];
 	index += 1;
 	sequenceNumber |= (navdata_message[index] << 8);
 	index += 1;
 	sequenceNumber |= (navdata_message[index] << 16);
 	index += 1;
 	sequenceNumber |= (navdata_message[index] << 24);
 	index += 1;
 	//printf("seq_num: %x\n", sequenceNumber);

 	visionFlag = navdata_message[index];
 	index += 1;
 	visionFlag |= (navdata_message[index] << 8);
 	index += 1;
 	visionFlag |= (navdata_message[index] << 16);
 	index += 1;
 	visionFlag |= (navdata_message[index] << 24);
 	index += 1;
 	//printf("Vision Flag: %x\n", visionFlag);

 	while (index != -1) {
 		getNextOption(&index);
 	}

}

void NavData::getNextOption(int *index) {
 	int16_t id;
 	int16_t option_size = 0;
 	id = (uint8_t)navdata_message[*index];
 	*index += 1;
 	id |= ((uint8_t)navdata_message[*index] << 8);
 	*index += 1;
 	//printf("id: %x\n");
 	option_size = (uint8_t)navdata_message[*index];
 	*index += 1;
 	option_size |= ((uint8_t)navdata_message[*index] << 8);
 	*index += 1;
 	//printf("size: %x\n", option_size);
 	//int i;
 	//for(i = *index + option_size - 4; i < 500; i++) printf("%x ", navdata_message[i]);
 	//printf("\n");
 	//sleep(1);
 	switch (id) {
 		case 0: //navdata_demo
 			//printf("\nDEMO\n");
 			parse_navdemo(index);
 			break;
 		case -1: //checksum
 			//printf("\nCHECK\n");
 			parse_checksum(index);
 			break;
 		default:
 			//printf("\nDEFAULT\n");
 			*index += option_size -4;
 			 break;
 	}
 }

 void NavData::parse_navdemo(int *index) {
 	int32_t temp;
 	navdemoData.ctrl_state = navdata_message[*index];
 	*index += 1;
 	navdemoData.ctrl_state |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	navdemoData.ctrl_state |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	navdemoData.ctrl_state |= (navdata_message[*index] << 24);
 	*index += 1 ;

 	navdemoData.vbat_flying_percentage = navdata_message[*index];
 	*index += 1 ;
 	navdemoData.vbat_flying_percentage |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	navdemoData.vbat_flying_percentage |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	navdemoData.vbat_flying_percentage |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	//printf("vbat_: %d%%\n", navdemoData.vbat_flying_percentage);

 	temp = navdata_message[*index];
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.theta = *(float *)&temp;
 	navdemoData.theta /= 1000;

 	temp = navdata_message[*index];
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.phi = *(float *)&temp;
 	navdemoData.phi /= 1000;

 	temp = navdata_message[*index];  /* THIS IS THE BLOCK THAT SETS HEADING */
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.psi = *(float *)&temp;
 	navdemoData.psi /= 1000;
 	//printf("YAW: %f \n", navdemoData.psi);

 	navdemoData.altitude = navdata_message[*index];
 	*index += 1 ;
 	navdemoData.altitude |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	navdemoData.altitude |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	navdemoData.altitude |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	//printf("alt: %f in\n", navdemoData.altitude);

 	temp = navdata_message[*index];
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.vx = *(float *)&temp;
 	//printf("vx: %f\n", navdemoData.vx);

 	temp = navdata_message[*index];
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.vy = *(float *)&temp;
 	//printf("vy: %f\n", navdemoData.vy);

 	temp = navdata_message[*index];
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 8);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 16);
 	*index += 1 ;
 	temp |= (navdata_message[*index] << 24);
 	*index += 1 ;
 	navdemoData.vz = *(float *)&temp;
 	//printf("vz: %f\n", navdemoData.vz);

 
 	//skip over all the other junk for right now
 	*index += 108;
}



struct navdata_demo NavData::getNavDataDemo()
{
	return navdemoData;
}

bool NavData::handleError(int i) {
	switch (i) {
		case SOCKET_FAIL:
		case BIND_FAIL:
		case SENDTO_FAIL:
		case RECVFROM_FAIL:
			return FATAL_ERROR;
			break;
		default:
			break;
	}
}

void NavData::parse_checksum(int *index) {
	*index = -1;
}
