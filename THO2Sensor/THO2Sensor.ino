/*
 *  Arduino ESP with THO2(Temperature/Humidity) sensor for Sensorflare.
 *
 *  This example demonstrates how to use
 *  connect a pressure sensor with Sensorflare
 *  using an ESP module running Arduino.
 *
 *  Created by Lidia Pocero.
 */

#include "Arduino.h"
#include "Wire.h" 

//Inlcude the necessary library for your specific sennso
#include <Digital_Light_TSL2561.h>
#include <TH02_dev.h>

//Include the 2 necessary library for HTTPS Wifi comunication
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//Your Sensorflare acreditation (username:password) on base64
#define CredBase64 "************************************"

// Unser name and password of you WiFi conection
const char* ssid = "*****";
const char* password = "*******";

//Variable used in order to try reconfigure the light sensor if had conexion problem
int problem;
//This variable will containt each time the name of the sensor to be update
String SensorName;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
   //Connect the device to the WiFi 
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


  //SDA, SCL I2C digital sensor connection 
  Wire.begin(2,14);
  
  // Power up,delay 150ms,until voltage is stable 
  delay(150);

  TH02.begin();
  delay(150);

}

void loop() {


   //Read temperature
   float t = TH02.ReadTemperature(); 
   Serial.println("Temperature: ");   
   Serial.print(t);
   Serial.println("C\r\n");
   
   //Send temperature if the temperature sensor is connect and the I2C comunication is correct
   if (t>-50)
   {
    //Name of the specific sensor that has bee
     SensorName = "mydev1/temperature";
     //Call the function to send to SensorFLare the before reading value 
     //The parameter to pass are the variable with the last read that you want to send "light" 
     //and the name of the sensor on sensorflare "SensorName"
     SendToSensorFlare(t, SensorName);
 
   }
   else //If the sensor is not connecte or have a problem comunication the value is not send
   Serial.println("Problem with the temperature sensor");
   delay(10000);


  //Calculate humidity

   float h = TH02.ReadHumidity();
   Serial.println("Humidity: ");
   Serial.print(h);
   Serial.println("%\r\n");

   //Send the Humidity value
   if (h>0)
   {
    //Select the name of the sensor on Sensorflare
    SensorName = "mydev1/humidity";
    //Call the function to send to SensorFLare the before reading value 
    //The parameter to pass are the variable with the last read that you want to send "light" 
    //and the name of the sensor on sensorflare "SensorName"
    SendToSensorFlare(h, SensorName);
   }
   else
   {
   Serial.println("Problem with the humidity sensor");
   }
   
   delay(10000);
 
}
//A Function that makes the HTTPS communication with the SensorFlare server to send each sensor value
//You just call this function on the loop to upload each sensor value on Sensorflare
void SendToSensorFlare(float var,String Name)
{
  //Construct the url 
  String url="/api/resource/"+Name+"?value=";
  
    //Sensorflare host
  const char* host = "www.sensorflare.com";
  const int httpsPort = 443;
  
  //Declare a client
    WiFiClientSecure client;
    
  //Connect to the Sensorflare server
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  
 //HTTPS protocol to POST to the server a value
  Serial.print("requesting URL: ");
  Serial.println(url);
  
  client.print("POST ");
  client.print( url );
  client.print(var,2);
  client.println(" HTTP/1.0");
  client.print("Host: ");
  client.println(host);
  client.print("Authorization: Basic ");
  client.println(CredBase64);
  client.println("User-Agent: espagent");
  client.println();
    
  delay(500);

 // To control and show the answer from the server
 Serial.println("request sent");
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
  
  client.stop();
  Serial.println("Client close");
  
}

