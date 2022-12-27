
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ThingSpeak.h>


//Define the relay pins
#define relay1 D0 //4,7
#define relay2 D1
#define BLYNK_TEMPLATE_ID "TMPLwxxg_rEt"
#define BLYNK_DEVICE_NAME "Relay"
#define DHTPIN D8
#define DHTTYPE DHT11
#define BLYNK_PRINT Serial

#define BLYNK_AUTH_TOKEN "Blynk_AUTH_TOKEN" //Enter your blynk auth token
DHT dht(DHTPIN, DHT11);

char auth[] = "Blynk_auth_token";
char ssid[] = "YOUR WIFI NAME";//Enter your WIFI name
char pass[] = "YOUR WIFI PASS";//Enter your WIFI password
WiFiClient  client;

unsigned long myChannelNumber = ChannelNUMBER;
const char * myWriteAPIKey = "Thingspeak API key";

//Get the button values
BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 1) {
    digitalWrite(relay1, LOW);
  } else {
    digitalWrite(relay1, HIGH);
  }
}

//Get the button values
BLYNK_WRITE(V1) {
  bool value2 = param.asInt();
  // Check these values and turn the relay2 ON and OFF
  if (value2 == 1) {
    digitalWrite(relay2, LOW);
  } else {
    digitalWrite(relay2, HIGH);
  }
}

void setup() {

  dht.begin();
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);

  // Turn OFF the relay
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);

  //Initialize the Blynk library
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  ThingSpeak.begin(client);
}

void loop() {
  //Run the Blynk library
  Blynk.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float m = analogRead(A0);



  ThingSpeak.setField(1, h); //It sets up fields for these three variables
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, m);





  int x = ThingSpeak.writeField(myChannelNumber, 1, h, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(5000); // Wait 30 seconds to update the channel again




  // write to the ThingSpeak channel
  int y = ThingSpeak.writeField(myChannelNumber, 2, t, myWriteAPIKey);
  if (y == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(y));
  }
  delay(5000); // Wait 30 seconds to update the channel again




  // write to the ThingSpeak channel
  int z = ThingSpeak.writeField(myChannelNumber, 3, m, myWriteAPIKey);
  if (z == 200) {
    Serial.println("Channel update successful.");
  }
  else {
    Serial.println("Problem updating channel. HTTP error code " + String(z));
  }
  delay(5000);


  Blynk.virtualWrite(V5, h);  //It assings the data on a virtual pin which is picked up by Blynk server and shown on the Blynk app
  Blynk.virtualWrite(V6, t);
  Blynk.virtualWrite(V13, m);
}
