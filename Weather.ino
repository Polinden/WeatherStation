#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoOTA.h>
#include <GyverTimer.h>
#include <TimeLib.h>
#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <GyverFilters.h>
#include "index.html.h"


#define STASSID "***"
#define STAPSK  "***"
#define EVERY   300000
#define URL "https://api.openweathermap.org/data/2.5/weather?q="
#define URL_TAIL "&units=metric&appid=*****"
#define URL_CORE "https://api.openweathermap.org"
#define CITY "Kyiv"
#define OTAPAS "***"

//web data
const char* ssid     = STASSID;
const char* password = STAPSK;
const char* url = URL;
const char* url_tail = URL_TAIL;
const char* host = URL_CORE;
String city {CITY};
IPAddress ip(192,168,77,226); 
IPAddress gateway(192,168,77,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8, 8, 8, 8); 
IPAddress secondaryDNS(192,168,77,1); 


AsyncWebServer server(80);
GKalman tempFilter(40, 40, 0.5);
GKalman presFilter(40, 40, 0.5);
GTimer myTimer(MS, EVERY);
WiFiClientSecure client;
HTTPClient http; 
DynamicJsonDocument doc(3000);
char buffer[500];
float o_tem=0.0;
float o_pres=0.0;
bool doReq=false;
  

void setup() {
  Serial.begin(9600);
  WiFi.disconnect(true);
  if (!WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure WiFi");
  }
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.println(WiFi.localIP());
  
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/html", index_html::HtmlPage);
  });  
  
  server.on("/set", HTTP_GET, [](AsyncWebServerRequest *request){
      if (request->hasParam("city")) city = request->getParam("city")->value();        
      doReq=true;
      request->redirect("/");   
  });  
  server.begin();
  doRequest();
  ArduinoOTA.setHostname("mish_weather_st"); 
  ArduinoOTA.setPassword(String(OTAPAS).c_str());
  ArduinoOTA.begin();
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
  char * s1; if (o_tem>tem) s1="["; if (o_tem<tem) s1="]"; o_tem=tempFilter.filtered(tem);
  char * s2; if (o_pres>pres) s2="["; if (o_pres<pres) s2="]"; o_pres=presFilter.filtered(pres);
  snprintf(buffer, sizeof(buffer), "In %s %s, %.1f`%s, wind %.1fm/s, %dmmHg%s, # %d:%02d \0", 
  n.c_str(), w.c_str(), tem, s1, spe, pres, s2, hour(timestm), minute(timestm));     
  Serial.println(buffer);  
}


void doRequest(){  
  client.setInsecure();
  client.connect(host, 443); 
  char my_url [300]={""}; 
  strcat(my_url, url);
  strcat(my_url, city.c_str());
  strcat(my_url, url_tail);
  //Serial.println(my_url);
  if (http.begin(client, my_url)) {
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
  doReq=false;
}

void loop() {
  ArduinoOTA.handle();  
  if (myTimer.isReady() || doReq) doRequest();
}
