#include <Arduino.h>
#include <I2C.h>
#include <Wire.h>
#include <avr/pgmspace.h>
// #include <image.h>
// #include <main.h>
// #include "video2led.h"
#include <SD.h>
#include <SdFat.h>
#define WAITMILLIS 500

#define CS_PIN 7
SdFat SD;
// SdFile file; // Déclare un objet fichier pour gérer les fichiers

void setup()
{
    // #ifdef DEBUG_VIDEO
    // #ifdef WAIT_SERIAL
    Serial.begin(115200);
    while (!Serial)
        ;
    // #endif
    delay(100);
    Serial.println("Serial ok\n");

    // Remapping des broches
    SPI.setTX(10);  // GPIO 3 pour MOSI
    Serial.println("setTX\n");
    SPI.setRX(9);  // GPIO 4 pour MISO
    Serial.println("setRX\n");
    SPI.setSCK(8); // GPIO 2 pour SCK
    Serial.println("setSCK\n");
    // Wire.setSCL(SCL);
    // Wire.setSDA(SDA);
    // Wire.begin(); // I2C init
    // Wire.setClock(400000UL); // I2C 800kHz
    //   IS31FL3737B_init();
    // randomSeed(analogRead(1));
    // pinMode(13, OUTPUT);
    // analogWrite(13, LED_LEVEL);
    // initExp(); // initialisation de la fonction exponentielle
    // Initialisation de la carte SD
    if (!SD.begin(CS_PIN))
    {
        Serial.println("Erreur lors de l'initialisation de la carte SD");
        while (1)
            ;
    }
    Serial.println("Initialisation SD réussie.");
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

    //   allLedPWMfull();
    //   delay(500);
    //   resetALlLedPWM();
    // memset(stack_dummy, 0xAA, STACK_SIZE);
    // check_stack_usage();
}

void loop()
{
    // noiseTest();
    //   readAndProcessFileBinaryBasicOneFrame("image_data_binary.bin");
    while (1)
        ;
}