/*
 *  ESP board Arduino program to receive through Serial Port the sensor names
 *  and values to be uploaded on Sensorflare
 *
 *  Created by Lidia Pocero.
 */

//Include the 2 necessary library for HTTPS Wifi comunication
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//Your Sensorflare acreditation (username:password) on base64
#define CredBase64 "**************************************"

// Unser name and password of you WiFi conection
const char* ssid = "********";
const char* password = "***********";


void setup()
{
  //The serial port will comunicate with th Arduino Module
  Serial.begin(9600);

   //Connect the device to the WiFi 
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
  delay (1000);
}

void loop()
{
  //Serial Comunication with the Arduino to take the sensor values to upload it 
  int incoming = Serial.available();
 
  if (incoming > 0)
  {
     
     //Necessary variables
      int indexL,indexH;
      String element,value;
      float val;
      String variable=Serial.readString();
      int len=variable.length();
  
      Serial.println(variable);
      int in=0; 
      if (variable!=0)
      {
        Serial.println("NEW MESSEGE");
        do{
           Serial.print("NEW VARIABLE:");
    
           indexL=variable.indexOf(':',in);
           element=variable.substring(in,indexL);
           indexH=variable.indexOf('\n',indexL);       
           value=variable.substring(indexL+1,indexH);
           val=value.toFloat(); 
  
         
           Serial.print(element);
           Serial.print(":");
           Serial.println(val);

           //Upload on Sensorflare each receive Sensor value
           SendToSensorFlare(val,element);
           in=indexH+1; 
        }while(in<len);
     }
  }

}

//Fucntion to Upload a sensor value on Sensorflare
void SendToSensorFlare(float var,String SensorName)
{

    String url="/api/resource/"+SensorName+"?value=";
    
      //Sensorflare host
    const char* host = "www.sensorflare.com";
    const int httpsPort = 443;
    
    WiFiClientSecure client;
    //Connect to the server
    Serial.print("connecting to ");
    Serial.println(host);
    if (!client.connect(host, httpsPort)) {
        Serial.println("connection failed");
        return;
      }
    
      
    //Connect to the server   
      Serial.print("requesting URL: ");
      Serial.println(url);
          // This will send the POST to the server
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
      delay(500);
} 

