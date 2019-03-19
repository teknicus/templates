
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 5;
constexpr uint8_t SS_PIN = 4;

byte nuidPICC[4];

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 // put your setup code here, to run once:

}

void loop() {
 
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  Serial.println("");
  for (byte i = 0; i < 8; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.print(nuidPICC[i], HEX);
    Serial.print("  |  ");
    }
   Serial.println("");
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}   
