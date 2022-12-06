#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#define  CLK 20
#define  DT  21

unsigned char flag, preVal, curVal;

int rotaryDeal(int counter)
{
	preVal = digitalRead(CLK);

	while(!digitalRead(DT)) {
		curVal = digitalRead(CLK);
		flag = 1;
	}

	if(flag == 1) {
		flag = 0;
		if((preVal == 0)&&(curVal == 1)) {
			counter++;	
			printf("%d\n", counter);
		}
		if((preVal == 1)&&(curVal == 0)) {
			counter--;
			printf("%d\n", counter);
		}
	}
	return counter;
}

int main(void)
{
	int counter = 0;
	
	if(wiringPiSetupGpio() < 0) {
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}

	pinMode(CLK, INPUT);
	pinMode(DT, INPUT);
	
	while(1) {
		counter = rotaryDeal(counter);
	}
	return 0;
}
