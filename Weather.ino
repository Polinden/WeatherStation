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
const char* url = "https://api.openweathermap.org/data/2.5/weather?q=Kyiv&units=metric&appid=***";
const char* host = "https://api.openweathermap.org";

GTimer myTimer(MS, 120000);
WiFiClientSecure client;
HTTPClient http; 
DynamicJsonDocument doc(10000);
  


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {delay(500);}  
  ArduinoOTA.begin();
  doRequest();
}



void pretiPrint(String s){
  DeserializationError error = deserializeJson(doc, s);
  if (error) Serial.println("...");    
  String w1=doc["weather"][0]["main"].as<String>();
  String w2=doc["main"]["temp"].as<String>();
  String w3=doc["main"]["pressure"].as<String>();
  String w4=doc["wind"]["speed"].as<String>();
  long timestm=doc["sys"]["sunset"].as<long>();
  int h=hour(timestm)+2;
  int m=minute(timestm);
  
  String s1=w1 + ", ";
  String s2="temp: "+w2 +"C, ";
  String s3=w3 +"Mbar, ";
  String s4="wind: "+w4+"m/s, ";  
  String s5="Sunset at:"+String(h)+":"+String(m)+" ";  
  String res="Hi, Michael! Today we have "+ s1+s2+s3+s4+s5;
  Serial.println(res);  
}


void doRequest(){  
  client.setInsecure();
  client.connect(host, 443);  
  if (http.begin(client, url)) {
     //http.addHeader("x-rapidapi-key", "50681c346emsha4ad7c2b5963babp151b34jsne2dab573e283");
     //http.addHeader("x-rapidapi-host", "community-open-weather-map.p.rapidapi.com");
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
