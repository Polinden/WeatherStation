#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoOTA.h>
#include <GyverTimer.h>
#include <TimeLib.h>
#include <ArduinoJson.h>


#define STASSID "****"
#define STAPSK  "****"
#define CITY    "Kyiv"
#define EVERY   300000

const char* ssid     = STASSID;
const char* password = STAPSK;
const char* url = "https://api.openweathermap.org/data/2.5/weather?q="CITY"&units=metric&appid=83e8e8aaff2ad065a44e1cd558f1b0a9";
const char* host = "https://api.openweathermap.org";

GTimer myTimer(MS, EVERY);
WiFiClientSecure client;
HTTPClient http; 
DynamicJsonDocument doc(2400);
char buffer[500];
  


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
  String w=doc["weather"][0]["description"].as<String>();
  String n=doc["name"].as<String>();
  float tem=doc["main"]["temp"].as<float>();
  int pres=doc["main"]["pressure"].as<int>()*0.75;
  float spe=doc["wind"]["speed"].as<float>();
  int tzone=doc["timezone"].as<int>();  
  long timestm=doc["sys"]["sunset"].as<long>()+tzone;  
  snprintf(buffer, sizeof(buffer), "Hi, Michael! In %s today is %s, temp %.1fC, wind %.1fm/s, %dmmHg, sunset at %d:%d \0", 
  n.c_str(), w.c_str(), tem, spe, pres, hour(timestm), minute(timestm));     
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
