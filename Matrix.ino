
#include <AltSoftSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "RTClib.h"
#include "GyverTimer.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  10
#define CS_PIN    7

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
RTC_DS1307 rtc;


#define BUF_SIZE 100
char curMessage[BUF_SIZE] = { "" };
char dateNow[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "" };
char daysOfTheWeek[7][4] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
bool newMessageAvailable = true;
bool my_turn=true;
GTimer gTimer1(MS, 5000);
GTimer gTimer2(MS, 60000);
AltSoftSerial altser;



void readSerial(void)
{   
  static char *cp = newMessage;

  while (altser.available())
  {
    *cp = (char)altser.read();
    if ((*cp == '\n') || (cp - newMessage >= BUF_SIZE-2)) 
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
  strcpy(dateNow,"%d.%d.%d, %s, %d:%d\0");
  snprintf(dateNow, strlen(dateNow), now.month(), now.month(), now.year(), daysOfTheWeek[now.dayOfTheWeek()], now.hour(), now.minute());
}


void setup()
{
  Serial.begin(9600);
  altser.begin(9600);
  rtc.begin();
  P.begin();
  P.displayText(curMessage, PA_LEFT, 70, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  readTime();
}




void loop()
{
  if (P.displayAnimate())
  {
    if (my_turn) P.displayText(dateNow, PA_CENTER, 100, 10, PA_SCROLL_DOWN, PA_SCROLL_UP)
    else P.displayText(curMessage, PA_LEFT, 70, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);      
   
    if (newMessageAvailable)
    {      
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  readSerial();
}
