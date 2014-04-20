#include "UDP.h"

void UDP::reset()
{
	seq = 0;
	char buff[100];
	sprintf(buff, "AT*COMWDG=%d\r", 0);
	navdata_write (buff);
}

void UDP::sendCommand(int commandCode)
{

	switch(commandCode)
	{
		case TAKEOFF:
			takeoff();
			break;
		case FORWARD:
			forward();
			break;
		case LEFT:
			left();
			break;
		case RIGHT:
			right();
			break;
		case DRONE_HOVER:
			hover();
			break;
		case ROTATE:
			rotate();
			break;
		case LAND:
			land();
			break;
		case WATCHDOG:
			watchDog();
			break;
		case CALIBMAGNETO:
			calib_magneto();
			break;
		case GETNAVDATA:
			getNavData();
			break;
		case BACK:
			back();
			break;
		case UP:
			up();
			break;
		case DOWN:
			down();
			break;
		case OROTATE:
			orotate();
			break;
		case ENDEMERGENCY:
			endemergency();
			break;
		case SWITCHCAMERA:
			switchcamera();
			break;
		default:
			getNavData();
			//should there be some error message
			//these are the questions that keep me up at night
			break;
	}
}

void UDP::up()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,0,%d,0\r", seq++, *(int*)&orotPower);
	printf("Going up\n");
	navdata_write (buff); 
}

void UDP::down()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,0,%d,0\r", seq++, *(int*)&rotPower);
	printf("Going down\n");
	navdata_write (buff);
}

void UDP::takeoff()
{
	char buff[100];
	sprintf(buff, "AT*FTRIM=%d\r", seq++);
	navdata_write (buff);
	sprintf(buff, "AT*REF=%d,290718208\r", seq++);
	printf("Takeoff sent yolo\n");
	navdata_write (buff);
}

void UDP::forward()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,%d,0,0\r", seq++, *(int*)&forwardPower);
//	sprintf(buff, "AT*PCMD=1,1,0,1036831949,0,0");
	printf("Forward sent");
	navdata_write (buff);
}

void UDP::back()
{
	float temp = forwardPower * -1;
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,%d,0,0\r", seq++, *(int*)&temp);
	navdata_write (buff);
}

void UDP::left()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,%d,0,0,0\r", seq++, *(int*)&tilt_left);
	navdata_write (buff);

}

void UDP::right()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,%d,0,0,0\r", seq++, *(int*)&tilt_right);
	navdata_write (buff);
}

void UDP::rotate()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,0,0,%d\r", seq++, *(int*)&rotPower);
	navdata_write (buff);
	printf("orotate\n");
}

void UDP::orotate()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,1,0,0,0,%d\r", seq++, *(int*)&orotPower);
	navdata_write (buff);
} 

void UDP::hover()
{
	char buff[100];
	sprintf(buff, "AT*PCMD=%d,0,0,0,0,0\r", seq++);
	printf("Hover sent\n");
	navdata_write (buff);
}

void UDP::land()
{
	char buff[100];
	sprintf(buff, "AT*REF=%d,290717696\r\0", seq++);
	printf("Land command sent...\n");
	navdata_write (buff);
}

void UDP::endemergency()
{
	char buff[100];
	sprintf(buff, "AT*REF=%d,290717952\r", seq++);
	printf("Endemergency sent...\n");
	navdata_write (buff);
}

void UDP::switchcamera()
{
	char buff[100];
	static int cameraStatus = 1;
	cameraStatus = 1 - cameraStatus;
	sprintf(buff, "AT*CONFIG=%d,\"video:video_channel\",\"%d\"\r", seq++, cameraStatus+1);
	printf("Switching camera..\n");
	navdata_write (buff);
}

void UDP::watchDog()
{
	char buff[100];
	sprintf(buff, "AT*COMWDG=%d\r", seq++);
	navdata_write (buff);
}

void UDP::calib_magneto()
{
	char buff[100];
 	sprintf(buff, "AT*CALIB=%d,1,\r", seq++);
 	navdata_write (buff);
}

void UDP::getNavData(void) {
 	char buff[100];
 	sprintf(buff,"AT*CONFIG=%d,\"general:navdata_demo\",\"TRUE\"\r",seq++);
 	navdata_write(buff);
 }

void UDP::setup(void) {
	seq = 0;
	int32_t zero=0;
	at_socket = -1;

	forwardPower = -.2;
	rotPower = -.5;
	orotPower = .5;
	tilt_left = -.3;
	tilt_right = .3;

 	if((at_socket = socket (AF_INET, SOCK_DGRAM, 0)) < 0){
         	printf ("at_socket error: %s\n", strerror(errno));
         	throw SOCKET_FAIL;
     	}


     	//for sendto AT
    drone_at.sin_family = AF_INET;
    drone_at.sin_addr.s_addr = inet_addr(WIFI_MYKONOS_IP);
    drone_at.sin_port = htons(AT_PORT);
	

}

void UDP::navdata_write(char *command) {
	int error = sendto(at_socket, command, strlen(command), 0, (struct sockaddr*)&drone_at, sizeof(drone_at) );
	if (error == -1) {
		throw SENDTO_FAIL;
	}
}

bool UDP::handleError(int i) {
	switch (i) {
		case SOCKET_FAIL:
			return FATAL_ERROR;
			break;
		case SENDTO_FAIL:
			return FATAL_ERROR;
			break;
		default:
			break;
		
	}
}

