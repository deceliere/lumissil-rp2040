#include "video2led.h"
#include "main.h"
#include <SD.h>
// #include <SdFat.h>
#include <I2C.h>
#include <Wire.h>

// SdFile file; // Déclare un objet fichier pour gérer les fichiers

const int bufferSize = W_SOURCE * H_SOURCE; // pixel q
// matrixDot dot[192];

// void readAndProcessFileBinaryBasicOneFrame(const char *filename)
void readAndProcessFileBinaryBasicOneFrame(const char *filename)
{
  // byte binSourceCurrent[W_SOURCE * H_SOURCE]; // Buffer pour stocker une ligne de pixels
  byte binSourceCurrent[192]; // Buffer pour stocker une ligne de pixels
  byte binDataArray[192];
  memset(binSourceCurrent, 10, 192);
  memset(binDataArray, 10, 192);
  // File file = SD.open(filename);
  // SD.end();
  // noInterrupts();
  // interrupts();
  // File file = SD.open(filename);

  IS_I2C_BufferWrite(binDataArray, 192, 0, Addr_GND_GND);
  // IS31FL3737B_init();

  // Wire.begin();
  // digitalWrite(SDCARD, HIGH); // Libère la ligne CS après l'ouverture
  // while (1)
  //   ;
  // elapsedMillis frameDelay = 0;
  // frameCount = 0;
  // #ifdef SERIAL_DEBUG
  //   printExpoScale();
  // #endif
  File file = SD.open(filename);
  if (file)
  {
    file.read(binSourceCurrent, 32);
    memcpy(binDataArray, binSourceCurrent, 32);
    // displayFrameBinaryBasic(binSourceCurrent);
    DPRINTLN("file closed");
    file.close();
    printDataBin(binSourceCurrent, 32);
    DPRINTLN("binDataArray:");
    printDataBin(binDataArray, 32);
    IS_I2C_BufferWrite(binDataArray, 32, 0, Addr_GND_GND);
  }
  else
  {
    Serial.println("Erreur lors de l'ouverture du fichier");
  }
  // frameCount++;
  // DPRINT("min value: ");
  // DPRINTLN(valueMin);
  // DPRINT("max value: ");
  // DPRINTLN(valueMax);
  // DPRINT("frametimer (microS) min: ");
  // DPRINTLN(frameTimerMin);
  // DPRINT("frametimer (microS) max: ");
  // DPRINTLN(frameTimerMax);
  // DPRINT("total microS variation span: ");
  // DPRINTLN(frameTimerMax - frameTimerMin);
  // DPRINT("total frame count: ");
  // DPRINTLN(frameCount);
  // DPRINT("loop duration (S): ");
  // DPRINTLN(loopDur / 1000);
}
/*
void displayFrameBinaryBasic(byte binArray[W_SOURCE * H_SOURCE])
{
  int i = 0;
  byte buffer[192];
  clearBuffer(buffer, 192);

  for (int y = 0; y < 12; y++)
  {
    DPRINT(">>y");
    DPRINTLN(y);
    for (int x = 0; x < 12; x++)
    {
      DPRINT("x");
      DPRINTLN(x);
      binaryToDotBasic(x, y, binArray, i);
      // if (dot[i].pwm < valueMin)
      //   valueMin = dot[i].pwm;
      // if (dot[i].pwm > valueMax)
      //   valueMax = dot[i].pwm;
      // IS_IIC_WriteByte(Addr_GND_GND, 0xFE, 0xc5); // unlock FDh
      // IS_IIC_WriteByte(Addr_GND_GND, 0xFD, 0x01); // Turn to page 1: PWM registers
      writeToBuffer(buffer, dot[i]);
      i++;
    }
  }

  printBuffer(buffer);
  // for(int i = 0; i < 192; i++)
  // {
  //   DPRINT("buffer[");
  //   DPRINT(i);
  //   DPRINT("]=");
  //   DPRINT(buffer[i]);
  //   DPRINT(", ");
  // }
  IS_I2C_BufferWrite(buffer, 192, 0, Addr_GND_GND);
  DPRINTLN("displayFrameBin");
}

void binaryToDotBasic(int x, int y, byte binArray[W_SOURCE * H_SOURCE], int i)
{
  dot[i].row = y;
  dot[i].col = x;
  // dot[i].pwm = binArray[x + X_OFFSET + (y * W_SOURCE + (W_SOURCE * Y_OFFSET) + (12 - y))];
  dot[i].pwm = binArray[x + y];
  DPRINT("i=");
  DPRINT(i);
  DPRINT(" y=");
  DPRINT(y);
  DPRINT(" x=");
  DPRINT(x);
  DPRINT(" pwm=");
  DPRINT(dot[i].pwm);
  DPRINTLN(" ");
}
*/