#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <wiringSerial.h>

// Pin 번호 설정
#define CLK  19
#define DT   26            // 로터리 엔코더(Rotary encoder)
#define TRIG 15
#define ECHO 18            // 초음파(Ultrasonic)
#define LED1 20
#define LED2 21            // LED(Light-emitting diode)
#define BAUD_RATE 115200   // UART 통신(Bluetooth baud rate)
#define SLAVE_ADDR_01 0x48 // 조도 센서(slave address)

// 전역변수 정의 및 초기화
int curVal = 0; // 현재 값(조도 센서)
int digital[10] = { 0 }; // 디지털관 주차장 생성을 위한 1차원 배열
int global[10] = { 0 };  // 글로벌관 주차장 생성을 위한 1차원 배열
int library[10] = { 0 }; // 도서관 주차장 생성을 위한 1차원 배열

int segmentSelect = 1; // 로터리 엔코더를 통해 각 건물을 선택하기 위한 변수
int dn[] = { 4,22,10,23 }; // 4-digit 7-segment display 자리마다의 GPIO 번호
int segment_pin[] = { 17,9,25,7,13,27,24,8 }; // 4-digit 7-segment별 GPIO 번호
short digits_data1[14] = { 0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE4,0xFE,0xE6,0xFC,0xBC,0x1C,0x02 }; // 숫자, 문자, 모양 표기(16진수)

static const char* I2C_DEV = "/dev/i2c-1";     // I2C 연결을 위한 장치 파일
static const char* UART2_DEV = "/dev/ttyAMA1"; // UART2 연결을 위한 장치 파일

// 함수 프로토타입 선언
void* ultraSonic(void* number); // 초음파 센서 제어 함수
void* bluetooth();              // UART 통신(Blutetooth) 제어 함수
void* photoresistor();          // 조도 센서 제어 함수
void* rotaryEncoder();          // 로터리 엔코더 제어 함수
void* digitSegment();           // 4-digit 7-segment display 제어 함수

void pinInit();                                     // Pin Mode 초기화 함수
void ledControl(int i, int index);                  // LED 제어 함수
int calEmpty(int select);                           // 남은 주차자리 수 계산 함수
void displaySegment(int pos, int num);              // Digit segment display에 값 출력 함수
unsigned char serialRead(const int fd);             // 1byte 데이터 읽기 함수
void serialWriteBytes(const int fd, const char* s); // Multi byte 데이터 쓰기 함수

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex 초기화

int main()
{
    int sonicNumber = 0;
    pthread_t th1, th2, th3, th4, th5;

    if (wiringPiSetupGpio() < 0) { // BCM 기반 PIN 번호 사용
        fprintf(stderr, "Unable to setup wiringPi:%s\n", strerror(errno));
        return 1;
    }

    pinInit();

    pthread_create(&th1, NULL, ultraSonic, (void*)sonicNumber++);
    pthread_create(&th2, NULL, bluetooth, NULL);
    pthread_create(&th3, NULL, photoresistor, NULL);
    pthread_create(&th4, NULL, rotaryEncoder, NULL);
    pthread_create(&th5, NULL, digitSegment, NULL);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);
    pthread_join(th4, NULL);
    pthread_join(th5, NULL);

    pthread_mutex_destroy(&mutex); // Mutex 해제

    return 0;
}

// 초음파 센서 제어 함수
void* ultraSonic(void* number)
{
    int n = (int)number;
    int distance = 0;
    long start, end;

    while (1) { // TRIG: 초음파를 보냄, ECHO: 초음파를 받음
        digitalWrite(TRIG, 0);
        delayMicroseconds(10);
        digitalWrite(TRIG, 1);
        delayMicroseconds(10);
        digitalWrite(TRIG, 0);

        while (digitalRead(ECHO) == 0)
            start = micros();
        while (digitalRead(ECHO) == 1)
            end = micros();
        distance = (end - start) / 58; // 거리 계산 식(cm)

        if (distance < 25 && curVal > 40) 
            ledControl(0, n); // 주차 O 상태
        else
            ledControl(1, n); // 주차 X 상태

        printf("Distance(cm): %d\n", distance);
        delay(1000);
    }
}

// UART 통신(Blutetooth) 제어 함수
void* bluetooth()
{
    int fd_serial;
    char str[50] = "=========================\n";
    unsigned char dat, buffer[500];

    if ((fd_serial = serialOpen(UART2_DEV, BAUD_RATE)) < 0)
    {
        printf("Unable to open serial device.\n");
        exit(1);
    }

    while (1)
    {
        fflush(stdout); // 버퍼 비우기
        switch (serialRead(fd_serial))
        {
        case 'D':
            sprintf(buffer, "%s|  디지털관 남은 주차자리 수 : %d  |\n%s",str, calEmpty(1), str);
            break;
        case 'G':
            sprintf(buffer, "%s|  글로벌관 남은 주차자리 수 : %d  |\n%s",str, calEmpty(2), str);
            break;
        case 'L':
            sprintf(buffer, "%s|   도서관 남은 주차자리 수 : %2d   |\n%s",str, calEmpty(3), str);
            break;
        default:
            continue;
        }
        serialWriteBytes(fd_serial, buffer); // 버퍼 쓰기
        delay(10);
    }
}

// 조도 센서 제어 함수
void* photoresistor()
{
    int i2c_fd;
    int threshold = 50, adcChannel = 2; // 조도 임계 값 및 채널 설정(외부)

    if ((i2c_fd = wiringPiI2CSetupInterface(I2C_DEV, SLAVE_ADDR_01)) < 0) {
        printf("wiringPi2CSetup Failed: \n");
        exit(1);
    }

    while (1)
    {
        wiringPiI2CWrite(i2c_fd, 0x40 | adcChannel);
        curVal = wiringPiI2CRead(i2c_fd); // 현재 조도 센서 값 읽기

        if (curVal < threshold) // 임계 값보다 낮으면(밝으면)
            printf("Bright\n");
        else                    // 임계 값보다 높으면(어두우면)
            printf("Dark\n");
        delay(1000);
    }
}

// 로터리 엔코더 제어 함수
void* rotaryEncoder()
{
    unsigned char fl, p, crr;

    while (1)
    {
        p = digitalRead(CLK);

        while (!digitalRead(DT)) {
            crr = digitalRead(CLK);
            fl = 1;
        }

        if (fl == 1) {
            fl = 0;
            if ((p == 0) && (crr == 1)) { // 시계 방향(CW)
                if (segmentSelect < 3) segmentSelect++;
                printf("%d\n", segmentSelect);
            }
            if ((p == 1) && (crr == 0)) { // 반시계 방향(CCW)
                if (segmentSelect > 1) segmentSelect--;
                printf("%d\n", segmentSelect);
            }
        }
    }
}

// 4-digit 7-segment display 제어 함수
void* digitSegment()
{
    // 1번 segment : segmentSelect 값에 따라 'D, G, L' 표시(변동)
    // 2번 segment : '-' 표시(고정)
    // 3,4번 segment : 선택된 건물에 해당하는 남은 주차자리 수 표시(변동)
    while (1)
    {
        displaySegment(1, 9 + segmentSelect); // 1번 segment 표시
        delay(1);
        displaySegment(2, 13); // 2번 segment 표시
        delay(1);

        if (calEmpty(segmentSelect) == 10)
        {
            displaySegment(3, 1); // 3번 segment 표시
            delay(1);
            displaySegment(4, 0); // 4번 segment 표시
            delay(1);
        }
        else
        {
            displaySegment(3, 0); // 3번 segment 표시
            delay(1);
            displaySegment(4, calEmpty(segmentSelect)); // 4번 segment 표시
            delay(1);
        }
    }
}

// Pin Mode 초기화 함수
void pinInit()
{
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
        {
            pinMode(dn[i], OUTPUT);
        }
        pinMode(segment_pin[i], OUTPUT);
    }
}

// LED 제어 함수
void ledControl(int i, int index)
{
    if (i == 0) // 주차 O 상태
    {
        digitalWrite(LED1, LOW);  // 파란색 LED OFF
        digitalWrite(LED2, HIGH); // 빨간색 LED ON

        switch (segmentSelect) // 현재 선택된 건물
        {
        case 1: // 디지털관
            digital[index] = 1; // 해당 배열 index 자리가 주차 상태임을 나타냄
            break;
        case 2: // 글로벌관
            global[index] = 1; // 해당 배열 index 자리가 주차 상태임을 나타냄
            break;
        case 3: // 도서관
            library[index] = 1; // 해당 배열 index 자리가 주차 상태임을 나타냄
            break;
        default:
            break;
        }
    }
    else if (i == 1) // 주차 X 상태
    {
        digitalWrite(LED1, HIGH); // 파란색 LED ON
        digitalWrite(LED2, LOW);  // 빨간색 LED OFF

        switch (segmentSelect) // 현재 선택된 건물
        {
        case 1: // 디지털관
            digital[index] = 0; // 해당 배열 index 자리가 출차 상태임을 나타냄
            break;
        case 2: // 글로벌관
            global[index] = 0; // 해당 배열 index 자리가 출차 상태임을 나타냄
            break;
        case 3: // 도서관
            library[index] = 0; // 해당 배열 index 자리가 출차 상태임을 나타냄
            break;
        default:
            break;
        }
    }
}

// 남은 주차자리 수 계산 함수
int calEmpty(int select)
{
    int cnt = 0;

    // 남은 자리 계산 중 접근 차단
    pthread_mutex_lock(&mutex); //Mutex Lock

    for (int i = 0; i < 10; i++)
    {
        if (select == 1) cnt += digital[i];
        else if (select == 2) cnt += global[i];
        else if (select == 3) cnt += library[i];
    }

    pthread_mutex_unlock(&mutex); //Mutex Unlock

    return 10 - cnt;
}

// Digit segment display에 값 출력 함수
void displaySegment(int pos, int num)
{
    for (int i = 0; i < 4; i++)
    {
        if (i + 1 == pos) digitalWrite(dn[i], LOW);
        else digitalWrite(dn[i], HIGH);
    }

    for (int i = 0; i < 8; i++)
    {
        short segmentData = (digits_data1[num] & (0x01 << i)) >> i;
        if (segmentData == 1) digitalWrite(segment_pin[7 - i], HIGH);
        else digitalWrite(segment_pin[7 - i], LOW);
    }
}

// 1byte 데이터 읽기 함수
unsigned char serialRead(const int fd)
{
    unsigned char x;
    if (read(fd, &x, 1) != 1) return -1;
    return x;
}

// Multi byte 데이터 쓰기 함수
void serialWriteBytes(const int fd, const char* s)
{
    write(fd, s, strlen(s));
}