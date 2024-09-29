#define BLYNK_TEMPLATE_ID "TEMPLATE-ID"  //Fill
#define BLYNK_TEMPLATE_NAME "TEMPLATE-NAME" //Fill
#define BLYNK_AUTH_TOKEN "ENTER-TOKEN" //Fill

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "WIFI-NAME"; //Fill
char pass[] = "WIFI-PASS"; //Fill

#define MPU_addr 0x68 // MPU6050 I2C address

int servoPin = 13;
Servo servo1;

int RST_PIN = 32;
int SS_PIN = 21;

int redLED = 25;
int greenLED = 26;
int blueLED = 27;

unsigned long previousMillis = 0;
const long interval = 30;

int X_offset = 0;
int Y_offset = 0;
int Z_offset = 1700;

MFRC522 rfid(SS_PIN, RST_PIN);
byte ID[4] = {163, 28, 255, 233}; // Your specific card UID

String authorizedUID = "A31CFFE9";

// Blynk virtual button state
bool buttonState = true;

BLYNK_WRITE(V6) {
  buttonState = param.asInt();
}

BLYNK_WRITE(V13) {
  servo1.write(param.asInt());
}

void writeConsole() {
  Serial.print("ID Number: ");
  for (int num = 0; num < 4; num++) {
    Serial.print(rfid.uid.uidByte[num]);
    Serial.print(" ");
  }
  Serial.println("");
}

void setup() {
  Serial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);

  servo1.attach(servoPin);

  Blynk.begin(auth, ssid, pass);  

  Serial.println("Scan RFID card...");
}

void loop() {
  Blynk.run();

  if (buttonState) {
    SPI.begin();
    rfid.PCD_Init();

    // RFID Reading
    digitalWrite(blueLED, HIGH);

    if (!rfid.PICC_IsNewCardPresent())
      return;

    if (!rfid.PICC_ReadCardSerial())
      return;

    String uid = "";
    for (byte i = 0; i < rfid.uid.size; i++) {
      uid += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      uid += String(rfid.uid.uidByte[i], HEX);
    }
    uid.toUpperCase();

    Serial.println("RFID UID: " + uid);
    Blynk.virtualWrite(V1, uid); // Send RFID UID to Blynk application

    if (uid == authorizedUID) {
      Serial.println("Accessed!");
      writeConsole();
      digitalWrite(greenLED, HIGH);
      digitalWrite(blueLED, LOW);
      Blynk.virtualWrite(V2, "Authorized Card Detected");
      servo1.write(90); // Move the servo to 90 degrees
      delay(3000);
      digitalWrite(greenLED, LOW);
    } else {
      Serial.println("Unauthorized Authority!");
      writeConsole();
      digitalWrite(redLED, HIGH);
      digitalWrite(blueLED, LOW);
      Blynk.virtualWrite(V2, "Unauthorized Card Detected");
      delay(3000);
      digitalWrite(redLED, LOW);
    }
    digitalWrite(blueLED, HIGH);
    rfid.PICC_HaltA();

  } else {
    // MPU6050 Reading
    Wire.begin();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B);  // Wakes up the MPU-6050
    Wire.write(0x00);  
    Wire.endTransmission(true);

    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // Starting with register 0x3B (ACCEL_XOUT_H)
      Wire.endTransmission(false);
      Wire.requestFrom(MPU_addr, 6, true); // Request a total of 6 registers

      int16_t XAxisFull = (Wire.read() << 8 | Wire.read()) + X_offset;  
      int16_t YAxisFull = (Wire.read() << 8 | Wire.read()) + Y_offset;  
      int16_t ZAxisFull = (Wire.read() << 8 | Wire.read()) + Z_offset; 
      float XAxisFinal = (float)XAxisFull / 16384.0;  
      float YAxisFinal = (float)YAxisFull / 16384.0;  
      float ZAxisFinal = (float)ZAxisFull / 16384.0;  

      if (XAxisFinal > 0.99) XAxisFinal = 1; 
      if (YAxisFinal > 0.99) YAxisFinal = 1;
      if (ZAxisFinal > 0.99) ZAxisFinal = 1;

      if (XAxisFinal < -0.99) XAxisFinal = -1;
      if (YAxisFinal < -0.99) YAxisFinal = -1;
      if (ZAxisFinal < -0.99) ZAxisFinal = -1;

      Serial.print("X Axis = ");
      Serial.print(XAxisFinal);
      Serial.print("g\t");
      Serial.print("Y Axis = ");
      Serial.print(YAxisFinal);
      Serial.print("g\t");
      Serial.print("Z Axis = ");
      Serial.print(ZAxisFinal);
      Serial.println("g");

      String accelData = String(XAxisFinal, 2) + "," + String(YAxisFinal, 2) + "," + String(ZAxisFinal, 2);
      Blynk.virtualWrite(V3, accelData); // Write combined XYZ axis value to Blynk
    }
  }
  delay(100);
}
