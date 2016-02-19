/*
 *  Include secure the sensors values of your deice on SensorFlare
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access by HTTPS API  the SensorFlare 
 *  platform and update sensor values on it 
 *
 *  Created by Lidia Pocero, 2016.
 *  This example is in public domain.
 */

#include "Arduino.h"
#include "Wire.h" 

//Include the necessary library for your specific sensor
#include <Digital_Light_TSL2561.h>

//Include the 2 necessary libraries for HTTPS Wifi communication
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//Your Sensorflare accreditation (username:password) on base64
#define CredBase64 "bGlkaWFwZjBAZ21haWwuY29tOkBuMG1AbEBzMTk4NQ=="

// User name and password of your WiFi connection
const char* ssid = "rota";
const char* password = "protarota67";

  
  void setup() {
    
      Serial.begin(115200);
      Serial.println();
      Serial.print("connecting to ");
      Serial.println(ssid);
      
      //Connect the device to the WiFi 
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
     
     //Digital Light Sensor initialization
      TSL2561.init();
   
  }
  
  void loop() {
        
       //Light sensor

      //Introduce as sensorname the name that you give to your sensor on Sensorflare
      String SensorName = "mydev1/light";

       //Read the lux value of the light sensor
      float light=TSL2561.readVisibleLux();
      Serial.print("The Light value is: ");
      Serial.println(light);
      delay(150); 
    
      //Send the light sensor variable only if give back correct value
      if (light>-1)
      {
        //Call the function to send to SensorFLare the before reading value 
        //The parameter to pass are the variable with the last read that you want to send "light" 
        //and the name of the sensor on sensorflare "SensorName"
        SendToSensorFlare(light, SensorName);
      }
      //Here each 60 seconds will be repeat the reading and send the sensor variable process
      delay(60000);
    
  }

  //A Function that makes the HTTPS communication with the SensorFlare server to send each sensor value
  void SendToSensorFlare(float var,String SensorN)
  {
    //Construct the url   
    String url="/api/resource/"+SensorN+"?value=";
  
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
  
    client.stop();
    Serial.println("Client close");
    
  }

