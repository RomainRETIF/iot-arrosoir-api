#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <math.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ArduinoJson.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>


#define USE_SERIAL Serial
#define NUMPIXELS 10
#define DELAYVAL 50
#define DHTPIN 14
#define DHTTYPE    DHT22

WiFiMulti wifiMulti;

const int ledPin = 26;
//const int sensor_dirt_humidity_pin = 34;
const int pompe_pin = 15;
int dirt_humidity;
float seuilLumiere;
float seuilTemperature;
float seuilHumidite;
float Rsensor;
bool estDeclencheLumiere = true;
bool estDeclenchePompe = true;
float temp;
float hum;


int test = 0;
int minuteur;

Adafruit_NeoPixel pixels(10, ledPin, NEO_GRB + NEO_KHZ800);

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(pompe_pin, OUTPUT);
  wifiMulti.addAP("ROMAIN", "75-P77y7");
  pixels.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);

  delayMS = sensor.min_delay / 1000;
  minuteur = 60;
}

void loop() {

  //REQUETES POUR LES SEUILS
  if (minuteur == 60) {
    if ((wifiMulti.run() == WL_CONNECTED)) {

      HTTPClient http;
      http.begin("http://192.168.1.100:3000/api/parametres"); //HTTP
      http.addHeader("Content-Type", "application/json");
      //String httpRequestData = "{\"luminosite\":0.5,\"humidite\":30,\"temperature\":45,\"duree_arrosage\":2}";
      //int httpResponseCode = http.POST(httpRequestData);
  
      int httpResponseCode = http.GET();
      http.end();
      int httpCode = http.GET();
  
      if (httpCode > 0) {
          String response = http.getString();
          Serial.print(response);
          DynamicJsonDocument doc(1024);
          deserializeJson(doc, response);
          seuilTemperature = doc[0]["temperature"];
          seuilLumiere = doc[0]["luminosite"];
          seuilHumidite = doc[0]["humidite"];
          Serial.print("ok");
      }
    }
    minuteur = 0;
  }
  //FIN REQUETES POUR LES SEUILS


  int capteurLumiere = analogRead(34);
  Rsensor = (float)(1023 - capteurLumiere) * 10 / capteurLumiere;
  sensors_event_t event;


  dht.temperature().getEvent(&event);
  temp = event.temperature;
  dht.humidity().getEvent(&event);
  hum = event.relative_humidity;
  //LUMIERE
  if (Rsensor > seuilLumiere)
  {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(150, 150, 150));

      pixels.show();
      delay(DELAYVAL);
    }
    if(estDeclencheLumiere == false){
      sendDeclenchement(temp, hum, Rsensor, 2); 
      estDeclencheLumiere = true;
    }
  }
  else
  {
    estDeclencheLumiere = false;
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));

      pixels.show();
      delay(DELAYVAL);
    }
  }
  //FIN LUMIERE


  //HUMIDITE & TEMPERATURE
  delay(delayMS);

  if (temp > seuilTemperature && hum < seuilHumidite) {
    digitalWrite(pompe_pin, HIGH);
    if(estDeclenchePompe == false){
      sendDeclenchement(temp, hum, Rsensor, 2); 
      estDeclenchePompe = true;
    }
    delay(2000);
    digitalWrite(pompe_pin, LOW);
  }
  else {
    estDeclenchePompe = false;
  }
  //capteur de la terre
  //dirt_humidity = analogRead(sensor_dirt_humidity_pin);
  //dirt_humidity= map(dirt_humidity,550,0,0,100);
  //if(dirt_humidity > 50){
  //digitalWrite(pompe_pin, LOW);
  //}
  //else{
  //digitalWrite(pompe_pin, HIGH);

  //FIN HUMIDITE & TEMPERATURE


  minuteur = minuteur + 1;
  delay(1000);
}

void sendDeclenchement(float temperature, float humidite, float luminosite, float duree_arrosage){
  
if ((wifiMulti.run() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin("http://192.168.1.100:3000/api/historique/add"); //HTTP
    http.addHeader("Content-Type", "application/json");

    DynamicJsonDocument doc(1024);
    doc["luminosite"] = luminosite;
    doc["humidite"]   = humidite;
    doc["temperature"] = temperature;
    doc["duree_arrosage"] = duree_arrosage;
    
    String response;
    
    serializeJson(doc, response);
    
    int httpResponseCode = http.POST(response);
    
    http.end();
    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.print("sent");
    }
  }  
}
