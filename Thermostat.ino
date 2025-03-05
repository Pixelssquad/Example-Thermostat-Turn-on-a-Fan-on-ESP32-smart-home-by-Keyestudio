//**********************************************************************************
/*  
 * Filename    : xht11_vent
 * Description : Read the temperature and humidity values of XHT11.
 * Auther      : http//www.keyestudio.com
 * 05/03/2025 added control of the fan if temp (or humidity)  rise up the selected value //By Diego "Diegooz" Pellacani
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);
#include "xht11.h"
xht11 xht(17);
#include <analogWrite.h>
#define fanPin1 19
#define fanPin2 18
#define led_y 12  //Define the yellow led pin to 12

unsigned char dht[4] = {0, 0, 0, 0};//Only the first 32 bits of data are received, not the parity bits
void setup() {
  Serial.begin(9600);//Start the serial port monitor and set baud rate to 9600
  mylcd.init();
  mylcd.backlight();
  pinMode(led_y, OUTPUT);  //Set pin to output mode
}

void loop() {
  if (xht.receive(dht)) { //Returns true when checked correctly
    Serial.print("RH:");
    Serial.print(dht[0]); //The integral part of humidity, DHT [1] is the fractional part
    Serial.print("%  ");
    Serial.print("Temp:");
    Serial.print(dht[2]); //The integral part of temperature, DHT [3] is the fractional part
    Serial.println("C");
    
    mylcd.setCursor(0, 0);
    mylcd.print("T = ");
    mylcd.print(dht[2]);
    mylcd.setCursor(0, 1);
    mylcd.print("H = ");
    mylcd.print(dht[0]);
    //mylcd.clear();
    delay(200);
  } else {    //Read error
    Serial.println("sensor error");
  }


  if (dht[2] > 26 ) { //set here the temp, over this the fan is ON - Change dht[2] to dht[0] if you want use umidity instead temp.

  digitalWrite(fanPin1, HIGH); //pwm = 255
  analogWrite(fanPin2, 210);  
    mylcd.setCursor(9, 0);
    mylcd.print("Fan ON ");
    digitalWrite(led_y, HIGH);  //Also Light up the LED (pin 12, defined in line 16)
  }else{
  digitalWrite(fanPin1, LOW); //pwm = 255
  analogWrite(fanPin2, LOW);
    mylcd.setCursor(9, 0);
    mylcd.print("Fan OFF");
    digitalWrite(led_y, LOW);  //Light up the LED 
    }
    mylcd.setCursor(11, 1);
    mylcd.print(";)");

  delay(1000);
}
//**********************************************************************************
