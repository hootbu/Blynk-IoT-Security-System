#define BLYNK_TEMPLATE_ID "TEMPLATE-ID"  //Fill
#define BLYNK_TEMPLATE_NAME "TEMPLATE-NAME" //Fill
#define BLYNK_AUTH_TOKEN "ENTER-TOKEN" //Fill

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#include <ESP32Servo.h>
#include <MFRC522.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI-NAME"; //Fill
char pass[] = "WIFI-PASS"; //Fill

int servoPin = 13;
Servo servo1;

int RST_PIN = 32;
int SS_PIN = 21;

MFRC522 rfid(SS_PIN, RST_PIN);

// Defined card UID
String authorizedUID = "A31CFFE9"; 

BLYNK_WRITE(V13){
  servo1.write(param.asInt());
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  servo1.attach(servoPin);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Scan RFID card...");
}

void loop() {
  Blynk.run();

  if (!rfid.PICC_IsNewCardPresent()) {
    return; //Exit if a new card is not read
  }

  if (!rfid.PICC_ReadCardSerial()) {
    return; //Exit if the serial number of the card could not be read
  }

  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println("RFID UID: " + uid);
  Blynk.virtualWrite(V1, uid); // Send RFID UID to Blynk application

  if (uid == authorizedUID) {
    Blynk.virtualWrite(V2, "Authorized Card Detected"); // Authorized card read, write message to V2
    servo1.write(90); // Move the servo to 90 degrees
  } else {
    Blynk.virtualWrite(V2, "Unauthorized Card Detected"); // Unauthorized card read, write message to V2
    servo1.write(0); // Move the servo to 0 degrees
  }

  rfid.PICC_HaltA(); //Stop reading
  rfid.PCD_StopCrypto1();
}
