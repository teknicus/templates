//unstable

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h> // Use the Library Manager to Install this
#include <PubSubClient.h>


const char* ssid = "--------";
const char* password = "--------";
const char* mqtt_server = "---------";//Brokers' ip address

constexpr uint8_t RST_PIN = 5;
constexpr uint8_t SS_PIN = 4;

const int card_addr = 0; // Array indices
const int keychain_addr = 1;
const int ID_addr = 2;
const int sticker_addr = 3;

int card_stat = 0; // Read Statuses
int keychain_stat = 0;
int ID_stat = 0;
int sticker_stat = 0;

boolean isKeychain = true; // boolean status check
boolean isCard = true;
boolean isID = true;
boolean isSticker = true;

MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;


byte nuidPICC[4];

byte card[4] = {0xFD, 0x21, 0x6, 0x85}; // Tag Data
byte keychain[4] = {0x65, 0x7A, 0x21, 0xC3};
byte ID[4] = {0x1E, 0xA5, 0xA1, 0x59};
byte sticker[4] = {0x4, 0x84, 0x6C, 0x42};

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  EEPROM.begin(512); // Allocating 512 bytes of EEPROM

  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting."); //Connecting to WiFi


  while (WiFi.status() != WL_CONNECTED) {

    delay(500); // Wait till it connects
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("Connected to ");
  Serial.print(ssid);
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());



  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("Callback");
}

void toggleF1() {

  Serial.println("card");
  card_stat = EEPROM.read(card_addr);

  if ( card_stat == 1 ) { //current state is on
    Serial.println("Sending OFF trigger");
    client.publish("armtronix_mqtt", "f1f");
    card_stat = 0;
  }
  else {
    Serial.println("Sending ON trigger");
    client.publish("armtronix_mqtt", "f1n");
    card_stat = 1;
  }

  EEPROM.write(card_addr, card_stat);
  EEPROM.commit();

  delay(3000);



}

void toggleF2() {

  Serial.println("keychain");

  
  keychain_stat = EEPROM.read(keychain_addr);

  if ( keychain_stat == 1 ) { //current state is on
    Serial.println("Sending OFF trigger");
    client.publish("armtronix_mqtt", "f3f");
    keychain_stat = 0;
  }
  else {
    Serial.println("Sending ON trigger");
    client.publish("armtronix_mqtt", "f3n");
    keychain_stat = 1;
  }

  EEPROM.write(keychain_addr, keychain_stat);
  EEPROM.commit();

  delay(3000);

}

void toggleF3() {

  Serial.println("ID");

    
  ID_stat = EEPROM.read(ID_addr);

  if ( ID_stat == 1 ) { //current state is on
    Serial.println("Sending OFF trigger");
    client.publish("armtronix_mqtt", "l1f");
    ID_stat = 0;
  }
  else {
    Serial.println("Sending ON trigger");
    client.publish("armtronix_mqtt", "l1n");
    ID_stat = 1;
  }

  EEPROM.write(ID_addr, ID_stat);
  EEPROM.commit();

  delay(3000);

}

void toggleL1() {

  Serial.println("Sticker");

 
  sticker_stat = EEPROM.read(sticker_addr);

  if ( sticker_stat == 1 ) { //current state is on
    Serial.println("Sending OFF trigger");
    client.publish("armtronix_mqtt", "l1f");
    sticker_stat = 0;
  }
  else {
    Serial.println("Sending ON trigger");
    client.publish("armtronix_mqtt", "l1n");
    sticker_stat = 1;
  }

  EEPROM.write(sticker_addr, sticker_stat);
  EEPROM.commit();


  delay(3000);
  
}

void loop() {

  digitalWrite(LED_BUILTIN, LOW);

  if (!client.connected()) {

    while(!client.connected()) 
{
    digitalWrite(LED_BUILTIN, HIGH); //LED goes off when pin pulled high.  Indicating there's no client connected
    Serial.print("Attempting MQTT connection...");
    if (client.connect("RFID_Node_1")) {
      Serial.println("connected");

      client.subscribe("armtronix_mqtt");
     client.publish("armtronix_mqtt", "RFID Node Connected");

    }

    if (client.connected()) { //verifying the connection
      Serial.println("Connected To Server");
      digitalWrite(LED_BUILTIN, LOW);
    }

    }

  }

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been read
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];

    if (nuidPICC[i] == card[i]) isCard = true;
    else isCard = false;

    if (nuidPICC[i] == keychain[i]) isKeychain = true;
    else isKeychain = false;

    if (nuidPICC[i] == ID[i]) isID = true;
    else isID = false;

    if (nuidPICC[i] == sticker[i]) isSticker = true;
    else isSticker = false;
  }



  if ( isCard == true ) {

    toggleF1();

  }

  else if ( isKeychain == true) {

    toggleF2();

  }

  else if (isID == true) {

    toggleF3();
  }

  else if (isSticker == true) {

    toggleL1();

  }


  client.loop();

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  
}
