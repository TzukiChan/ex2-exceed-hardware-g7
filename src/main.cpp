// If you want to run in WOKWi
// change pin and wifi
#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>
#include "traffic.h"

#define RED 26
#define YELLOW 25
#define GREEN 33
#define LDR 34
#define BUTTON 27

#define light analogRead(LDR)

int state = 1;
int count = 0;
Bounce debouncer = Bounce();

void Connect_Wifi();

void setup()
{
  Serial.begin(115200);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(LDR, INPUT);
  debouncer.attach(BUTTON, INPUT_PULLUP);
  debouncer.interval(25);
  Connect_Wifi();

  delay(200);
  // start LED with GREEN and POST to database
  digitalWrite(GREEN, 1);
  // Serial.println("green na");
  POST_traffic("green");
  GET_traffic();
  digitalWrite(YELLOW, 0);
  digitalWrite(RED, 0);
}

void loop()
{
  // *** write your code here ***
  // Your can change everything that you want

  if (state == 1)
  {
    // while led GREEN
    if (count == 0)
    {
      debouncer.update();
      if (debouncer.fell()) 
      { 
        count++;
        Serial.println(count);
      }
    }
    else if (count == 1)
    {
      digitalWrite(GREEN, 0);
      digitalWrite(YELLOW, 1);
      digitalWrite(RED, 0);
      // Serial.println("yellow na");
      POST_traffic("yellow");
      state += 1;
      count = 0;
    }
  }
  else if (state == 2)
  {
    // while led YELLOW
    delay (8000);
    digitalWrite(GREEN, 0);
    digitalWrite(YELLOW, 0);
    digitalWrite(RED, 1);
    // Serial.println("red na");
    POST_traffic("red");
    GET_traffic();
    state += 1;    
  }
  else if (state == 3)
  {
    // while led RED
    delay (5000);
    if (light < 3000)
    {
      digitalWrite(GREEN, 1);
      digitalWrite(YELLOW, 0);
      digitalWrite(RED, 0);
      // Serial.println("green na");
      POST_traffic("green");
      GET_traffic();
      state = 1;
    }
  }
}

void Connect_Wifi()
{
  const char *ssid = "nitro 5";
  const char *password = "12345687";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}