#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h> 
#include <ArduinoOTA.h>
#include <GyverTimer.h>


#ifndef STASSID
#define STASSID "***"
#define STAPSK  "***"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
const char* url = "https://api.openweathermap.org/data/2.5/weather?q=Kyiv&units=metric&appid=****";
const char* host = "https://api.openweathermap.org";

GTimer myTimer(MS, 120000);
WiFiClientSecure client;
HTTPClient http; 
  


void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  ArduinoOTA.begin();
  doRequest();
}



void pretiPrint(String s){
  int i1 = s.indexOf("main")+7;
  int i2 = s.indexOf("description")-3;
  int i3 = s.indexOf("temp")+6;
  int i4 = s.indexOf("feels_like")-2;
  int i5 = s.indexOf("speed")+7;
  int i6 = s.indexOf("deg")-2;
  int i7 = s.indexOf("pressure")+10;
  int i8 = s.indexOf("humidity")-2;

  
  
  String s1=s.substring(i1,i2) + ", ";
  String s2="temp:" + s.substring(i3,i4)+"C, ";
  String s3=s.substring(i7,i8)+"Mbar, ";
  String s4="wind:" + s.substring(i5,i6)+"m/s ";  
  String res="Kiev today, "+ s1+s2+s3+s4;
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
        Serial.printf("GET... failed, error: %s\n", http.errorToString(httpCode).c_str());        
      }
  }
  else Serial.println("No connection!");  
  http.end();  
}

void loop() {
  ArduinoOTA.handle();
  if (myTimer.isReady()) doRequest();
}
