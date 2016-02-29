/*
 *  Arduino with THO2(Temperature/Humidity) and VOC sensors.
 *
 *  This example show how to send the Name and Value of each Sensor
 *  to ESP module through Serial Port.
 *
 *  Created by Lidia Pocero.
 */

//Libraries for I2C comunication with the THO
#include <Wire.h>
#include <TH02_dev.h>

//Estructure for each Sensor
struct Sensor {
  String Name;
  float Value;
};

//Definde your Sensor number
const int VarNumber = 3;

//Array of Sensors
Sensor MySensors[VarNumber];

void setup()
{
  Wire.begin();

  //Digital Sensors
  /* Power up,delay 150ms,until voltage is stable */
  delay(150);
  /* Reset HP20x_dev */
  TH02.begin();

  delay(150);
}

void loop()
{
  //TH02 sensor

  //Temperature value
  float t = TH02.ReadTemperature();
  //Sensor name: The name of the sensor on Sensorflare
  MySensors[0].Name = "mydev1/temperature";
  //Sensor value
  MySensors[0].Value = t;

  delay(150);

  //Humidity value
  float h = TH02.ReadHumidity();
  //Sensor name: The name of the sensor on Sensorflare
  MySensors[1].Name = "mydev1/humidity";
  //Sensor value
  MySensors[1].Value = h;
  delay(150);

  //VOC sensor

  int gas = analogRead(A2);
  float voc = gas * 4.9;
  //Sensor name: The name of the sensor on Sensorflare
  MySensors[2].Name = "mydev1/voc";
  //Sensor value
  MySensors[2].Value = voc;
  
  //Send all the Sensor Values to ESP
  SendToESP();


}
//Funcion that send the variable names and the variable values to the ESP
void SendToESP()
{
  int i = 0;
  char Val[10];
  Serial.begin(9600);
  while (i < VarNumber)
  {
    dtostrf(MySensors[i].Value, 4, 2, Val);
    Serial.print(MySensors[i].Name);
    Serial.print(":");
    Serial.println(Val);
    i++;
  }
  Serial.end();
  //Later of each send to ESP wait half minets before to go back
  delay(30000);

}

