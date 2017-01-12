#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <VL6180X.h>

char ssid[] = "FIT5140"; 
char pass[] = "fit5140password";

ESP8266WebServer server(80);
VL6180X sensor;

String webString = "";
uint8_t distance;

void setup(void)
{
  Serial.begin(115200);
  
  WiFi.begin(ssid, pass);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  
  Wire.begin(4, 5);
  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);

  server.on("/", [](){
    distance = sensor.readRangeSingleMillimeters();
    Serial.println(distance);
    if (distance > 100)
      webString = "OPEN";
    else
      webString = "CLOSED";
    server.send(200, "text/plain", webString);
  });
  
  server.begin();
  Serial.println("HTTP server started!");
}

void loop(void)
{
  server.handleClient();
}

