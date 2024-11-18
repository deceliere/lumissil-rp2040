#include <Arduino.h>
#include <I2C.h>
#include <Wire.h>
#include <avr/pgmspace.h>
#include <image.h>
#include <main.h>
#include "video2led.h"
#include <SD.h>
// #include <SdFat.h>
#define WAITMILLIS 500

// SdFat SD;
// SdFile file; // Déclare un objet fichier pour gérer les fichiers

void setup()
{
  // #ifdef DEBUG_VIDEO
#ifdef WAIT_SERIAL
  Serial.begin(115200);
  while (!Serial)
    ;
#endif
delay(100);
  DPRINT("Serial ok\n");
  // pinMode(28, OUTPUT);
  // pinMode(29, INPUT_PULLUP);
  Wire.setSDA(26);
  Wire.setSCL(27);
  // Wire.setSDA(0);
  // Wire.setSCL(1);
  Wire.begin(); // I2C init
  // Wire.setClock(400000UL); // I2C 800kHz
  IS31FL3737B_init();
  randomSeed(analogRead(1));
  // pinMode(13, OUTPUT);
  // analogWrite(13, LED_LEVEL);
  // initExp(); // initialisation de la fonction exponentielle
  // Initialisation de la carte SD
  if (!SD.begin(SDCARD))
  {
    Serial.println("Erreur lors de l'initialisation de la carte SD");
    while (1)
      ;
  }
  Serial.println("Initialisation carte SD réussie.");
  // File myFile = SD.open("exemple.txt");
  // if (myFile)
  // {
  //   Serial.println("Contenu de exemple.txt :");
  //   while (myFile.available())
  //   {
  //     Serial.write(myFile.read());
  //   }
  //   myFile.close();
  //   Serial.println();
  // }
  // else
  // {
  //   Serial.println("Erreur lors de l'ouverture du fichier exemple.txt");
  // }

  Serial.println("Recherche des périphériques I2C...");
  for (byte address = 1; address < 127; ++address)
  {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0)
    {
      Serial.print("Périphérique I2C trouvé à l'adresse 0x");
      Serial.println(address, HEX);
    }
  }

  allLedPWMfull();
  delay(500);
  resetALlLedPWM();
  // memset(stack_dummy, 0xAA, STACK_SIZE);
  // check_stack_usage();
}

void loop()
{
  // noiseTest();
  readAndProcessFileBinaryBasicOneFrame("image_data_binary.bin");
  while (1)
    ;
}

void IS_IIC_WriteByte(uint8_t Dev_Add, uint8_t Reg_Add, uint8_t Reg_Dat)
{
  Wire.beginTransmission(Dev_Add / 2); // transmit to device address
  Wire.write(Reg_Add);                 // sends register address
  Wire.write(Reg_Dat);                 // sends register data
  Wire.endTransmission();              // stop transmitting
}

void IS31FL3737B_init(void) // white LED
{
  uint8_t i;
  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // Unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x03); // Turn to page 3: function registers
  IS_IIC_WriteByte(Addr_GND_GND, 0x00, 0x00); // Enable software shutdown

  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // Unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x00); // Turn to page 0: control registers
  for (i = 0; i < 0x18; i = i + 1)
    IS_IIC_WriteByte(Addr_GND_GND, i, 0xff); // open all LED
  // delay(1000);
  // can use buffer write type as figure 7 in datasheet

  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x01); // Turn to page 1: PWM registers
  for (i = 0; i < 192; i++)
    IS_IIC_WriteByte(Addr_GND_GND, i, 0x00); // Set PWM data to 0
  // can use buffer write type as figure 7 in datasheet

  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5);           // unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x03);           // Turn to page 3: function registers
  IS_IIC_WriteByte(Addr_GND_GND, 0x01, GLOABL_CURRENT); // global current
  IS_IIC_WriteByte(Addr_GND_GND, 0x00, 0x11);           // Release software shutdown to normal operation
}

void allLedPWMfull(void)
{
  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x00); // write page 0
  for (int i = 0; i < 24; i++)
    IS_IIC_WriteByte(Addr_GND_GND, i, 0xFF); // open all led
  int led = 192;
  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x01); // Turn to page 1: PWM registers
  while (led--)
    IS_IIC_WriteByte(Addr_GND_GND, led, 255); // update all PWM
  PRINTLN("all led full");
}

void resetALlLedPWM(void)
{
  IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // unlock FDh
  IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x01); // Turn to page 1: PWM registers
  for (uint8_t i = 0; i < 192; i++)
    IS_IIC_WriteByte(Addr_GND_GND, i, 0x00); // Set PWM data to 0
  PRINTLN("all led OFF");
}

/* conversion de base 12 a base 16 */
int resolve(int led)
{
  return (led += (led / 6) * 2);
}

uint8_t IS_I2C_BufferWrite(uint8_t *pBuffer, int length, int WriteAddress, int DeviceAddress)
{
  int seg = length / 32; // six segments aka 192/32 (Wire transmission = max 32 bytes)
  for (int i = 0; i < seg; i++)
  {
    Wire.beginTransmission(DeviceAddress / 2); // transmit to device address
#ifdef SERIAL_DEBUG
    DPRINT("inital write return=");
    DPRINTLN(Wire.write(WriteAddress + (i * 32))); // sends register address
    DPRINT("write address=");
    DPRINTLN(WriteAddress + (i * 32));
#else
    Wire.write(WriteAddress + (i * 32)); // sends register address
#endif
    printBuffer(pBuffer, 32);
    for (int i = 32; i > 0; i--)
    {
      // DPRINT("BufferWrite ");
      // DPRINT("i=");
      // DPRINT(i);
      // DPRINT(" pBuffer=");
      // DPRINTLN(*pBuffer);
      // DPRINT("[pBuffer Write returns]");
      // DPRINTLN(Wire.write(*pBuffer++)); // sends register data
      Wire.write(*pBuffer++); // sends register data
    }
// check_stack_usage();
// delay(1000);
#ifdef SERIAL_DEBUG
    DPRINT("Wire.endTransmission returns:");
    DPRINTLN(Wire.endTransmission()); // stop transmitting
#else
    Wire.endTransmission(); // stop transmitting
#endif
  }
  return 1;
}

void printBuffer(uint8_t *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    DPRINT("buffer[");
    DPRINT(i);
    DPRINT("]= ");
    DPRINTLN(buffer[i]);
  }
}

void clearBuffer(uint8_t *buffer, int buffLength)
{
  for (uint8_t i = 0; i < buffLength; i++)
    buffer[i] = 0;
}

void writeToBuffer(uint8_t *buffer, matrixDot dot)
{
  buffer[resolve(dot.col) + resolve(dot.row * 12)] = dot.pwm;
}

void noiseTest(void)
{
  matrixDot dot;
  uint8_t buffer[192];
  while (1)
  {
    clearBuffer(buffer, 192);
    for (int col = 0; col < 12; col++)
    {
      for (int i = 0; i < 12; i++)
      {
        dot.col = col;
        dot.row = i;
        dot.pwm = random(255);
        writeToBuffer(buffer, dot);
      }
    }
    IS_I2C_BufferWrite(buffer, 192, 0, Addr_GND_GND);
    delay(100);
  }
}

void studipTest(void)
{
  matrixDot dot;
  uint8_t buffer[192];
  clearBuffer(buffer, 192);

  dot.col = 1;
  dot.row = 4;
  dot.pwm = 255;
  writeToBuffer(buffer, dot);
  IS_I2C_BufferWrite(buffer, 192, 0, Addr_GND_GND);
  while (1)
    ;
}

void printDataBin(byte *pBuffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    DPRINT("i[");
    DPRINT(i);
    DPRINT("]=");
    DPRINT(*pBuffer++);
    DPRINT(", ");
  }
  DPRINTLN();
}