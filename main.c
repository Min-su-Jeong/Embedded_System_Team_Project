#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <wiringPiI2C.h>

#define LED1 20
#define LED2 21
#define TRIG 15
#define ECHO 18
#define BAUD_RATE 115200
#define SLAVE_ADDR_01 0x48

static const char* UART2_DEV = "/dev/ttyAMA1";
static const char* I2C_DEV = "/dev/i2c-1";

void pinInit();
void switchControl(int i,int index);
void serialWrite(const int fd, const unsigned char c);
void serialWriteBytes(const int fd, const char* s);
unsigned char serialRead(const int fd);

void* aino();
void* bluetooth();
void* ultraSonic(void* number);

int digital[10] = {0};
int global[5] = {0};
int tech[3] = {0};
int space = 0;
int preVal = 0;
int curVal = 0;

int main()
{
	if(wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}
	pinInit();
	
	pthread_t th1, th2, th3;
	int sonicNumber = 0;
	
	pthread_create(&th1,NULL,ultraSonic,(void*)sonicNumber++);
	pthread_create(&th2,NULL,bluetooth,NULL);
	pthread_create(&th3,NULL,aino,NULL);
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);
	pthread_join(th3,NULL);
	
	return 0;
}

void pinInit()
{
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
}

void* ultraSonic(void* number)
{
	int n = (int)number;

	int distance = 0;
	long start,end;
	
	while(1) {
		digitalWrite(TRIG, 0);
		delayMicroseconds(10);
		digitalWrite(TRIG, 1);
		delayMicroseconds(10);
		digitalWrite(TRIG, 0);
		
		while(digitalRead(ECHO)==0)
			start = micros();
		while(digitalRead(ECHO)==1)
			end = micros();
		distance = (end-start) / 58;

		if (distance < 25)
			switchControl(0,n);	
		else
			switchControl(1,n);		
		printf("Distance(cm): %d\n", distance);
		delay(1000);
	}
}

void switchControl(int i, int index)
{
	if (i==1)
	{
		digitalWrite(LED1, HIGH);
		digitalWrite(LED2, LOW);
		
		if (space == 1)
		{
			digital[index] = 0;
			printf("ddd %d\n",digital[0]);
		}
		else if (space == 2)
		{
			global[index] = 0;
			printf("ggg %d\n",global);
		}
		else if (space == 3)
		{
			tech[index] = 0;
			printf("ttt %d\n",tech);
		}
	}
	else if (i==0)
	{
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, HIGH);
	
		if (space == 1)
		{
			digital[index] = 1;
			printf("ddd %d\n",digital[0]);
		}
		else if (space == 2)
		{
			global[index] = 1;
			printf("ggg %d\n",global);
		}
		else if (space == 3)
		{
			tech[index] = 1;
			printf("ttt %d\n",tech);
		}
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

void* bluetooth()
{
	int fd_serial;
	unsigned char dat;
	char buf[100];
	unsigned char buffer[100];
	
	if ((fd_serial = serialOpen(UART2_DEV,BAUD_RATE))<0)
	{
		printf("Unable to open serial device.\n");
		exit(1);
	}
	
	while(1)
	{
		dat = serialRead(fd_serial);
		printf("%c",dat);
		if(dat == 'D')
		{
			space = 1;
			fflush(stdout);
			sprintf(buffer, "Digital %d\n",digital[0]);
			serialWriteBytes(fd_serial, buffer);
		}
		else if(dat == 'G')
		{
			space = 2;
			fflush(stdout);
			sprintf(buffer, "Global\n");
			serialWriteBytes(fd_serial, buffer);
		}
		else if(dat == 'T')
		{
			space = 3;
			fflush(stdout);
			sprintf(buffer, "Global\n");
			serialWriteBytes(fd_serial, buffer);
		}
		else
			continue;
		fflush(stdout);
		delay(10);
	}
}

void* aino()
{
	int i2c_fd;
	int cnt = 0;
	int threshold = 50;
	int adcChannel = 2;
	
	if ((i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01)) < 0) {
		printf("wiringPi2CSetup Failed: \n");
		exit(1);
	}
	
	while(1)
	{
		wiringPiI2CWrite(i2c_fd,0x40|adcChannel);
		preVal = wiringPiI2CRead(i2c_fd);
		curVal = wiringPiI2CRead(i2c_fd);
	
		printf("[%d] Previous value  = %d, ",cnt,preVal);
		printf("Current value  = %d, ",curVal);
		
		if (curVal < threshold)
			printf("Bright\n");
		else
			printf("Dark\n");
		delay(1000);
		cnt++;
	}
}