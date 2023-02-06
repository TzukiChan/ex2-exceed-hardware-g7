#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Bounce2.h>

const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app";

const String point = "7";
const int nearby_1 = 6;
const int nearby_2 = 8;

void GET_traffic()
{
  DynamicJsonDocument doc(65536);
  HTTPClient http;
  const String url = baseUrl + "/all_traffic";
  http.begin(url);

  Serial.println("Nearby traffic");
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);
    Serial.println(httpResponseCode);

    Serial.print("point 6 : ");
    Serial.println((const char*)doc["all_traffic"][nearby_1-1]["point"]);
    Serial.println((const char*)doc["all_traffic"][nearby_1-1]["traffic"]);

    Serial.print("point self : ");
    Serial.println((const char*)doc["all_traffic"][6]["point"]);
    Serial.println((const char*)doc["all_traffic"][6]["traffic"]);

    Serial.print("point 8 : ");
    Serial.println((const char*)doc["all_traffic"][nearby_2-1]["point"]);
    Serial.println((const char*)doc["all_traffic"][nearby_2-1]["traffic"]);
    Serial.print("\n");

  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void POST_traffic(String led)
{
  const String url = baseUrl + "/my_traffic?point=" + point;
  String json;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  DynamicJsonDocument doc(2048);
  doc["code"] = "f0ter";
  doc["traffic"] = led;
  serializeJson(doc, json);

  Serial.println("POST " + led);
  int httpResponseCode = http.POST(json);
  Serial.println(httpResponseCode);
  if (httpResponseCode == 200)
  {
    Serial.print("Done");
    Serial.println();
  }
  else
  {
    Serial.print("Error  ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}