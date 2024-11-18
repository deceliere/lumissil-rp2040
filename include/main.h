#ifndef MAIN_H
#define MAIN_H

// #include "video2led.h"

// #define DEBUG_VIDEO
#define GLOABL_CURRENT 50
#define RANDOM_DROP_DUR 200
#define RANDOM_DROP_MIN 50
#define RANDOM_PAUSE_MIN 1000
#define RANDOM_PAUSE 20000
#define RANDOM_MAX_PWM 100
#define RANDOM_PWM_MIN 1
#define SDCARD 7 // SDcard CS pin on RP2040
#define ARDUINO_ARCH_AVR

#define LED_LEVEL 10

// #ifdef DEBUG
// #define DPRINT(x) Serial.print(x)
// #define DPRINTLN(x) Serial.println(x)
// #else
// #define DPRINT(x)
// #define DPRINTLN(x)
// #endif

#include <Arduino.h>
#include <elapsedMillis.h>
#include <iostream> 

typedef struct matrixDot
{
    uint8_t row; // 8 bits
    uint8_t col; // 8 bits
    uint8_t pwm; // 8 bits
    // uint8_t pwmTmp; // 8 bits
    // uint8_t pwmNext; // 8 bits
    // uint8_t pwmNextTmp; // 8 bits
    // uint8_t pwmFade; // 8 bits
    // uint16_t currentRandomTimer; // 16bits
    // uint16_t nextRandomTimer; // 16bits
    // elapsedMillis pixelTimer; // 32bits
    // int8_t postRow;
    // int8_t postPwm;

} matrixDot;

typedef struct dropTimer
{
    unsigned long prev;
    unsigned long current;
    unsigned long fadePrev;
    unsigned long fadeCurrent;
    uint16_t onDuration;
    uint16_t pauseDuration;
    uint16_t fadeDuration;
    bool active;
    matrixDot dot;
} dropTimer;


void I2C_allLedOn(void);
void IS31FL3737B_Test_mode1(void);
void IS31FL3737B_init(void);
void IS_IIC_WriteByte(uint8_t Dev_Add, uint8_t Reg_Add, uint8_t Reg_Dat);
uint8_t IS_I2C_BufferWrite(uint8_t *pBuffer, int length, int WriteAddress, int DeviceAddress);
void resetALlLedPWM(void);
void allLedPWMfull(void);
void breathAllLed(void);
void twelveBaseTest(void);
int resolve(int led);
void verticalTest(void);
void clearBuffer(uint8_t *buffer, int buffLength);
void writeToBuffer(uint8_t *buffer, matrixDot dot);
void writeFadeToBuffer(uint8_t *buffer, matrixDot dot);
void drop(void);
void bufferTest(void);
void printBuffer(uint8_t *buffer, int length);
void initDrop(dropTimer *drop);
void printDot(matrixDot dot);
uint16_t randomDrop(void);
uint16_t randomPause(void);
uint8_t randomPWM(void);

// xfade
void xfadeTest(void);
void xfadeDotInit(matrixDot *dot);

// tests
void studipTest(void);
void noiseTest(void);
void check_stack_usage(void);
void printDataBin(byte *pBuffer, int length);


#endif