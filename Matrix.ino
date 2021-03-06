
#include <AltSoftSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "RTClib.h"
#include "GyverTimer.h"
#include <Adafruit_SleepyDog.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define CLK_PIN   13
#define DATA_PIN  10
#define CS_PIN    7

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
RTC_DS3231 rtc;

#define BUF_SIZE1 100
#define BUF_SIZE2 30
char curMessage[BUF_SIZE1] = { "" };
char dateNow[3][BUF_SIZE2] = { "","","" };
char newMessage[BUF_SIZE1] = { "Weather is loading... # # #" };
char dateNew[3][BUF_SIZE2] = { "","","" };
char daysOfTheWeek[7][6] = {"Sund", "Mond", "Tuesd", "Wedn", "Thurs", "Frid", "Sat"};
bool newMessageAvailable = true;
bool newTimeAvailable = false;
int my_turn {0};
GTimer gTimer(MS, 60000);
AltSoftSerial altser;


uint8_t upR[] = { 8, 0, 0, 32, 18, 10, 6, 30, 0}; 
uint8_t downR[] = { 8, 0, 2, 4, 72, 80, 96, 120, 0}; 
uint8_t degC[] = { 8, 0, 60, 66, 66, 66, 0, 6, 6 }; 
uint8_t sunS[] = { 8, 162, 236, 144, 156, 147, 232, 164, 146 };


void readSerial(void)
{   
  static char *cp = newMessage;

  while (altser.available())
  {
    *cp = (char)altser.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE1-2)) 
    {
      *--cp = '\0';       
      cp = newMessage;
      newMessageAvailable = true;
    }
    else  
      cp++;
  }
}

void readTime(void)
{     
  DateTime now = rtc.now();
  snprintf(dateNew[0], BUF_SIZE2, "%d:%02d\0", now.hour(), now.minute());
  snprintf(dateNew[1], BUF_SIZE2, "%d/%d\0", now.day(), now.month());
  snprintf(dateNew[2], BUF_SIZE2, "%s\0", daysOfTheWeek[now.dayOfTheWeek()]);
  newTimeAvailable=true;
}


boolean timeSync(char * mes)
{     
   if (strstr(mes, "timesync")) {    
      char * z=strstr(mes, "=");      
      char t1[6],t2[6],t3[6],t4[6],t5[6],t6[6];      
      t1[0]=t2[0]=t3[0]=t4[0]=t5[0]=t6[0]='\0';
      strncat(t1, (z+1), 4); 
      strncat(t2, (z+5), 2); 
      strncat(t3, (z+7), 2); 
      strncat(t4, (z+9), 2); 
      strncat(t5, (z+11), 2); 
      strncat(t6, (z+13), 2);       
      rtc.adjust(DateTime(atoi(t1), atoi(t2), atoi(t3), atoi(t4), atoi(t5), atoi(t6)));
      return false;
   }
   return true;
}


void setup()
{
  Serial.begin(9600);
  altser.begin(9600);
  Serial.println( "Compiled: " __DATE__ ", " __TIME__ ", " __VERSION__);
  P.begin(2);
  P.setZone(0, 4, 7);
  P.setZone(1, 0, 3);
  P.setIntensity(0, 5);
  P.setIntensity(1, 5);
  //if (rtc.lostPower()) {
  //  Serial.println("RTC lost power, let's set the time!");
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //}
  rtc.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))+ TimeSpan(60));
  readTime();  
  P.addChar(1, ']', upR);
  P.addChar(1, '[', downR);
  P.addChar(1, '`', degC);
  P.addChar(1, '#', sunS);
  P.displayZoneText(1, curMessage, PA_LEFT, 80, 20, PA_SCROLL_LEFT, PA_SCROLL_LEFT);  
  Watchdog.enable(10000);
}




void loop()
{
  if (P.displayAnimate())
  { 
    if (P.getZoneStatus(1))
    {      
      if (newMessageAvailable) {
         if (timeSync(newMessage)) {
         strcpy(curMessage, newMessage);
         }
         newMessageAvailable = false;
      }   
      P.displayReset(1);
    }
    if (P.getZoneStatus(0))
    {      
      if (newTimeAvailable) {
         memcpy(dateNow, dateNew, sizeof dateNew); 
         newTimeAvailable = false;
      }
      P.displayZoneText(0, dateNow[my_turn++%3], PA_CENTER, 120, 2000, PA_GROW_UP, PA_GROW_UP);
    }
    
  }
  readSerial();
  if (gTimer.isReady()) readTime();
  Watchdog.reset();
}
