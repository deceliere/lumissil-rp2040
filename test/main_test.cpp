#include <Arduino.h>
// #include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>

// DFRobotDFPlayerMini myDFPlayer;
// void printDetail(uint8_t type, int value);

// 24.09.2024
// simple script pour mettre en boucle le DF mini player (si cela ne foncitonne pas avec le ADkey)
// sur XIAO RP2040

const int chipSelect = 7; // GPIO pour CS

void setup()
{
  // Serial.begin(115200);
  Serial1.begin(9600);
  while (!Serial)
  {
    ; // Attendre que la connexion soit établie
  }

  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, HIGH);
  digitalWrite(PIN_LED_G, HIGH);
  digitalWrite(PIN_LED_B, HIGH);
  delay(100);

  Serial.print("Initialisation de la carte SD...");
  if (!SD.begin(chipSelect))
  {
    Serial.println("Échec de l'initialisation !");
    return;
  }
  Serial.println("Initialisation réussie.");

  // Lire un fichier exemple
  File myFile = SD.open("exemple.txt");
  if (myFile)
  {
    Serial.println("Contenu de exemple.txt :");
    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  else
  {
    Serial.println("Erreur lors de l'ouverture du fichier exemple.txt");
  }
}

void loop()
{
  analogWrite(PIN_LED_G, 255);
  delay(1000);
  analogWrite(PIN_LED_G, 0);
  delay(1000);

  // if (myDFPlayer.available())
  // printDetail(myDFPlayer.readType(), myDFPlayer.read()); // Print the detail message from DFPlayer to handle different errors and states.
}

// void printDetail(uint8_t type, int value)
// {
//   switch (type)
//   {
//   case TimeOut:
//     Serial.println(F("Time Out!"));
//     break;
//   case WrongStack:
//     Serial.println(F("Stack Wrong!"));
//     break;
//   case DFPlayerCardInserted:
//     Serial.println(F("Card Inserted!"));
//     break;
//   case DFPlayerCardRemoved:
//     Serial.println(F("Card Removed!"));
//     break;
//   case DFPlayerCardOnline:
//     Serial.println(F("Card Online!"));
//     break;
//   case DFPlayerUSBInserted:
//     Serial.println("USB Inserted!");
//     break;
//   case DFPlayerUSBRemoved:
//     Serial.println("USB Removed!");
//     break;
//   case DFPlayerPlayFinished:
//     Serial.print(F("Number:"));
//     Serial.print(value);
//     Serial.println(F(" Play Finished!"));
//     break;
//   case DFPlayerError:
//     Serial.print(F("DFPlayerError:"));
//     switch (value)
//     {
//     case Busy:
//       Serial.println(F("Card not found"));
//       break;
//     case Sleeping:
//       Serial.println(F("Sleeping"));
//       break;
//     case SerialWrongStack:
//       Serial.println(F("Get Wrong Stack"));
//       break;
//     case CheckSumNotMatch:
//       Serial.println(F("Check Sum Not Match"));
//       break;
//     case FileIndexOut:
//       Serial.println(F("File Index Out of Bound"));
//       break;
//     case FileMismatch:
//       Serial.println(F("Cannot Find File"));
//       break;
//     case Advertise:
//       Serial.println(F("In Advertise"));
//       break;
//     default:
//       break;
//     }
//     break;
//   default:
//     break;
//   }
// }