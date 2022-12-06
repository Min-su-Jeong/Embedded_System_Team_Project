#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>

#define TRIG 15
#define ECHO 18

int main(void)
{
	int distance = 0;
	long start,end;
	
	if(wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}
	
	pinMode(TRIG, OUTPUT);
	pinMode(ECHO, INPUT);
	
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
		distance = ((end-start) / 58) * 10;
		printf("Distance(mm): %d\n", distance);
		delay(200); // 5Hz
	}
	return 0;
}
