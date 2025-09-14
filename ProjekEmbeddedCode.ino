#include <Servo.h>
#include <HX711_ADC.h>
#include <Wire.h>
#include "RTClib.h"

int jarak = 22;
int toleransi = 2;
int servo1 = 4;
int trig = 2;
int echo = 3;
int sck = 8;
int dt = 9;

int distance;
int length;
float berat;

Servo servo;
HX711_ADC scale(dt, sck); 
RTC_DS3231 rtc;

void setup() {
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  servo.attach(servo1);
  servo.write(90);

  scale.begin();
  delay(1000);
  scale.start(2000);
  scale.setCalFactor(137.7);

  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("RTC tidak terdeteksi!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, atur waktu...");
    rtc.adjust(DateTime(2025, 6, 3, 12, 0, 0));
  }
}

void loop() {
  DateTime now = rtc.now();

  // bool waktuMakan = (now.hour() == 7 || now.hour() == 17) && now.minute() == 0;

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  length = pulseIn(echo, HIGH);
  distance = (length / 2) / 29.1;

  scale.update();
  berat = scale.getData();

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.print(" cm, Berat: ");
  Serial.print(berat);
  Serial.println(" gram");

  // if (waktuMakan)
  // {
    if (distance >= jarak) 
    {
      Serial.println("Makanan habis.");
      servo.write(90);
    } 
    else if (berat < 7) 
    {
      Serial.println("Berat kurang dari 7 gram. Memberi makanan...");
      servo.write(110);  
      delay(1000);
      servo.write(90);   
    } 
    else 
    {
      servo.write(90);   
    }
  // }

  delay(1000);  // jeda antar loop
}
