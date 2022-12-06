#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

#define SLAVE_ADDR_01 0x48
static const char* I2C_DEV = "dev/i2c-1";

int main() {
	int i2c_fd;
	int cnt = 0;
	int preVal = 0;
	int curVal = 0;
	int threshold = 150;
	int adcChannel = 2;

	if (wiringPiSetupGpio() < 0) {
		printf("wiringPiSetup() is failed\n");
		return 1;
	}
	if ((i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01)) < 0) {
		printf("wiringPi I2C Setup Failed: \n");
		return 1;
	}
	printf("I2C start....\n");
	while (1) {
		wiringPiI2CWrite(i2c_fd, 0x00 | adcChannel);

		preVal = wiringPiI2CRead(i2c_fd);
		curVal = wiringPiI2CRead(i2c_fd);
		printf("[%d] Previous value = %d, ", cnt, preVal);
		printf("Current value = %d, ", curVal);

		if (curVal < threshold) printf("Bright!\n");
		else printf("Dark!\n");
		delay(500);
		cnt++;
	}
	return 0;
}