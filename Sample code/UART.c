#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BAUD_RATE 115200

#define LED 18
	
static const char* UART2_DEV = "/dev/ttyAMA1";

unsigned char serialRead(const int fd);
void serialWrite(const int fd,const unsigned char c);
void serialWriteBytes(const int fd,const char *s);


int main()
{
	int fd_serial;
	unsigned char dat;
	char buf[100];
	
	
	if (wiringPiSetupGpio()<0)	return 1;
	if ((fd_serial = serialOpen(UART2_DEV,BAUD_RATE))<0)
	{
		printf("Unable to open serial device.\n");
		return 1;
	}
	
	pinMode(LED, OUTPUT);
	
	while(1)
	{
		dat = serialRead(fd_serial);
		printf("%c",dat);
		if(dat == '1')
			digitalWrite(LED, LOW);
		else if(dat == '0')
			digitalWrite(LED, HIGH);
		else
			continue;
		fflush(stdout);
		delay(10);
	}

	
}

void serialWrite(const int fd,const unsigned char c)
{
	write(fd,&c,1);
}

unsigned char serialRead(const int fd)
{
	unsigned char x;
	if(read(fd,&x,1)!=1)
		return -1;
	return x;
}

void serialWriteBytes(const int fd,const char *s)
{
	write(fd,s,strlen(s));
}




