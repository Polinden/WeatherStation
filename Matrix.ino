
#include <AltSoftSerial.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  10
#define CS_PIN    7

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);


#define  BUF_SIZE  100
char curMessage[BUF_SIZE] = { "" };
char newMessage[BUF_SIZE] = { "Hello, Michael!" };
bool newMessageAvailable = true;
AltSoftSerial altser;
int buf_step=0;


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


void setup()
{
  Serial.begin(9600);
  altser.begin(9600);
  P.begin();
  P.displayText(curMessage, PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
}

void loop()
{
  if (P.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    P.displayReset();
  }
  readSerial();
}