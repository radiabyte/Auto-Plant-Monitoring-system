#include "DHT.h"
#define DHTPIN 8
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHT11);

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



SoftwareSerial espSerial(10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup()
{
  dht.begin();
  Serial.begin(115200);
  espSerial.begin(115200);
  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Temp: ");
  lcd.setCursor(8, 0);
  lcd.print("Hum: ");
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  
}

void loop()
{
  float h = 56; //dht.readHumidity();
  float t = dht.readTemperature();
  Serial.println(h);
  Serial.println(t);
  float moist = analogRead(A0);
  float m = ((100 - ( moist / 100 ) ) - 90) * (100/8);
  
  lcd.setCursor(5, 0);
  lcd.print(int(t));
  lcd.setCursor(12, 0);
  lcd.print(int(h));

  lcd.setCursor(9, 1);
  lcd.print(int(m));
  delay(5000);
  lcd.clear();
  lcd.print("Temp:");
  lcd.setCursor(8,0);
  lcd.print("Hum:");
  lcd.setCursor(0, 1);
  lcd.print("Moisture:");
  
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  data["hum"] = h;
  data["temp"] = t;
  data["moist"] = m;

  data.printTo(espSerial);

  jsonBuffer.clear();
  delay(2000);
}
