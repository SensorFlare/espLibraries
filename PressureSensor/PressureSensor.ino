/*
 *  Arduino ESP Pressure Sensor for Sensorflare.
 *
 *  This example demonstrates how to use
 *  connect a pressure sensor with Sensorflare
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
  //read the sensor value
  double val = analogRead(A0);

  String url = "/api/resource/esp/pressure?value=0";
  int cval = 0;
  if (val > 10) {
    cval=1;
    url = "/api/resource/esp/pressure?value=1";
  }
  if (prev!=cval){
    digitalWrite(0,HIGH);
    // Use WiFiClientSecure class to create TLS connection
    WiFiClientSecure client;
    Serial.print("connecting to ");
    Serial.println(host);
    if (client.connect(host, httpsPort)) {
        Serial.print("requesting URL: ");
        Serial.println(url);
        client.print(String("POST ") + url + " HTTP/1.1\r\n" +
                "Host: " + host + "\r\n" +
                "Authorization: Basic USERAUTHORIZATIONTOKEN\r\n" +
                "User-Agent: ESP8266SensorflareClient\r\n" +
                "Connection: close\r\n\r\n");
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
        digitalWrite(0,LOW);
    }
  }   
  delay(100);
}
