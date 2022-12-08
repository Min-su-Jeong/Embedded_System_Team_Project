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
#define CLK 19
#define DT  26

int dn[] = {4,22,10,23};
int segment_pin[] = {17,9,25,7,13,27,24,8};
short digits_data1[14] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE4,0xFE,0xE6,0xFC,0xBC,0x1C,0x02};

static const char* UART2_DEV = "/dev/ttyAMA1";
static const char* I2C_DEV = "/dev/i2c-1";
unsigned char fl, p,crr;

void pinInit(); // pinMode 초기화 함수
void switchControl(int i,int index); // LED 스위치 제어 함
void* ultraSonic(void* number); // 초음파 센서를 통한 LED 제어 함수
unsigned char serialRead(const int fd);
void serialWrite(const int fd,const unsigned char c);
void serialWriteBytes(const int fd,const char *s);
void* bluetooth();
void* aino();
void* rotaryDeal();
void show(int pos,int num);
int calEmpty(int select);
void* display();

int digital[10]  = {0};
int global[10] = {0};
int tech[10] = {0};
int preVal = 0;
int curVal = 0;
int segmentSelect = 1;

int main()
{
	if(wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}
	pinInit();
	
	pthread_t th1, th2, th3,th4,th5;
	int sonicNumber = 0;
	
	pthread_create(&th1,NULL,ultraSonic,(void*)sonicNumber++);
	pthread_create(&th2,NULL,bluetooth,NULL);
	pthread_create(&th3,NULL,aino,NULL);
	pthread_create(&th4,NULL,rotaryDeal,NULL);
	pthread_create(&th5,NULL,display,NULL);
	pthread_join(th1,NULL);
	pthread_join(th2,NULL);
	pthread_join(th3,NULL);
	pthread_join(th4,NULL);
	pthread_join(th5,NULL);
	
	return 0;
}

void pinInit()
{
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	
	for (int i =0;i<4;i++)
	{
		pinMode(dn[i],OUTPUT);
	}
	
	for (int i =0;i<8;i++)
	{
		pinMode(segment_pin[i],OUTPUT);
	}
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

		// 거리에 따른 서로 다른 LED 출력
		if (distance < 25 && curVal > 40)
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
		
		if (segmentSelect == 1)
		{
			digital[index] = 0;
		}
		else if (segmentSelect == 2)
		{
			global[index] = 0;
		}
		else if (segmentSelect == 3)
		{
			tech[index] = 0;
		}
	}
	else if (i==0)
	{
		digitalWrite(LED1, LOW);
		digitalWrite(LED2, HIGH);
	
		if (segmentSelect == 1)
		{
			digital[index] = 1;
		}
		else if (segmentSelect == 2)
		{
			global[index] = 1;
		}
		else if (segmentSelect == 3)
		{
			tech[index] = 1;
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
			fflush(stdout);
			sprintf(buffer, "디지털관 남은 주차자리 수 : %d\n",calEmpty(1));
			serialWriteBytes(fd_serial, buffer);
		}
		else if(dat == 'G')
		{
			fflush(stdout);
			sprintf(buffer, "글로벌관 남은 주차자리 수 : %d\n",calEmpty(2));
			serialWriteBytes(fd_serial, buffer);
		}
		else if(dat == 'L')
		{
			fflush(stdout);
			sprintf(buffer, "도서관 남은 주차자리 수 : %d\n",calEmpty(3));
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
		
		if (curVal < threshold)
			printf("Bright\n");
		else
			printf("Dark\n");
		delay(1000);
		cnt++;
	}
}

void* rotaryDeal()
{
	while(1)
	{
		p = digitalRead(CLK);

		while(!digitalRead(DT)) {
			crr = digitalRead(CLK);
			fl = 1;
		}

		if(fl == 1) {
			fl = 0;
			if((p == 0)&&(crr == 1)) {
				if (segmentSelect < 3) segmentSelect++;	
				printf("%d\n", segmentSelect);
			}
			if((p == 1)&&(crr == 0)) {
				if (segmentSelect > 1) segmentSelect--;
				printf("%d\n", segmentSelect);
			}
		}
	}
}

void* display()
{
	while(1)
	{
		show(1,9+segmentSelect);
		delay(1);
		show(2,13);
		delay(1);
		
		if (calEmpty(segmentSelect) == 10)
		{
			show(3,1);
			delay(1);
			show(4,0);
			delay(1);
		}
		else
		{
			show(3,0);
			delay(1);
			show(4,calEmpty(segmentSelect));
			delay(1);
		}

	}
}

void show(int pos,int num)
{
	for (int i =0;i<4;i++)
	{
		if(i+1==pos) digitalWrite(dn[i],LOW);
		else digitalWrite(dn[i],HIGH);
	}
	
	if (pos == 1)
	{
			for (int i =0;i<8;i++)
	{
		short segmentData = (digits_data1[num]&(0x01<<i)) >> i;
		if (segmentData==1)
			digitalWrite(segment_pin[7-i],HIGH);
		else
			digitalWrite(segment_pin[7-i],LOW);
		}
	}
	for (int i =0;i<8;i++)
	{
		short segmentData = (digits_data1[num]&(0x01<<i)) >> i;
		if (segmentData==1)
			digitalWrite(segment_pin[7-i],HIGH);
		else
			digitalWrite(segment_pin[7-i],LOW);
	}
}


int calEmpty(int select)
{
	int temp = 0;
	for (int i = 0;i<10;i++)
	{
		if (select==1) temp += digital[i];
		else if (select==2) temp += global[i];
		else if (select==3) temp += tech[i];
	}
	return 10-temp;
}
