/*
 *  Arduino ESP Soil Humidity Sensor for Sensorflare.
 *
 *  This example demonstrates how to use
 *  connect a soil humidity sensor with Sensorflare
 *  using an ESP module running Arduino.
 *
 *  Created by Dimitrios Amaxilatis.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "ssid";
const char* password = "password";

const char* host = "www.sensorflare.com";
const int httpsPort = 443;
int prev =  0 ;
void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(0,OUTPUT);
}


void loop() {
  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (client.connect(host, httpsPort)) {
    //read the sensor value
    double val = analogRead(A0);

    String url = "/api/resource/esp/plant1/humidity?value=";
    
    Serial.print("requesting URL: ");
    Serial.print(url);
    Serial.println(val);
    
    client.print(String("POST ") + url);
    client.print(val);
    client.print(String(" HTTP/1.1\r\n" )+
                "Host: " + host + "\r\n" +
                    "Authorization: Basic USERAUTHORIZATIONTOKEN\r\n" +
                    "User-Agent: ESP8266SensorflareClient\r\n" +
                "Connection: close\r\n\r\n");

    Serial.println("request sent");
    delay(1000);
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
        Serial.println("headers received");
        break;
        }
    }
    String line = client.readStringUntil('\n');
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    Serial.println("==========");
    Serial.println("closing connection");
  }
  delay(60000);
}
