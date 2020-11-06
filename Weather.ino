#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoOTA.h>
#include <GyverTimer.h>
#include <TimeLib.h>
#include <ArduinoJson.h>


#ifndef STASSID
#define STASSID "***"
#define STAPSK  "***"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
const char* url = "https://api.openweathermap.org/data/2.5/weather?q=Kyiv&units=metric&appid=83e8e8aaff2ad065a44e1cd558f1b0a9";
const char* host = "https://api.openweathermap.org";

GTimer myTimer(MS, 300000);
WiFiClientSecure client;
HTTPClient http; 
DynamicJsonDocument doc(10000);
char buffer[1000]={""};
  


void setup() {
  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500);} 
  ArduinoOTA.setHostname("mish_weather_st"); 
  ArduinoOTA.begin();
  doRequest();
}



void pretiPrint(String s){
  DeserializationError error = deserializeJson(doc, s);
  if (error) Serial.println("...");    
  String w1=doc["weather"][0]["description"].as<String>();
  String w2=doc["main"]["temp"].as<String>();
  int pres=doc["main"]["pressure"].as<int>()*0.75;
  String w3=doc["wind"]["speed"].as<String>();
  long timestm=doc["sys"]["sunset"].as<long>();  
  snprintf(buffer, sizeof(buffer), "Hi, Michael! Weather today is %s, temp %sC, wind %sm/s, %dmmHg, sunset at %d:%d \0", 
  w1.c_str(), w2.c_str(), w3.c_str(), pres, hour(timestm)+2, minute(timestm));     
  Serial.println(buffer);  
}


void doRequest(){  
  client.setInsecure();
  client.connect(host, 443);  
  if (http.begin(client, url)) {
     int httpCode = http.GET();      
      if (httpCode > 0) {        
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();          
          pretiPrint(payload);
        }
      } else {
        Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());        
      }
  }
  else Serial.println("No connection!");  
  http.end();  
}

void loop() {
  ArduinoOTA.handle();
  if (myTimer.isReady()) doRequest();
}
