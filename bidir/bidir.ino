#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include <DHT.h>

//Constants
#define DHTPIN 4    // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)
// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

Adafruit_LiquidCrystal lcd(12, 11, 9, 8, 7, 6);

//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

#define in 2
#define out 3

int count=0;
void IN()
{
    count++;
    lcd.setCursor(0, 0);
    lcd.print("Total people:");
    lcd.setCursor(13, 0);
    lcd.print(count);
    Serial.print("Person In Room:");
    Serial.println(count);
    delay(1000);
    
}
void OUT()
{
    count--;
    lcd.setCursor(0, 0);
     if(count<0){
    count=0;
    }
    lcd.print("Total people:");
    lcd.setCursor(13, 0);
    lcd.print(count);
    Serial.print("Person In Room:");
    Serial.println(count);
    delay(1000);
    lcd.clear();
}
void setup() {
  dht.begin();
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(in, INPUT);
  
  pinMode(out,INPUT);
  Serial.print("Person In Room:");
  Serial.print(count);
   pinMode(5,OUTPUT); 
}

void loop() {
  if(digitalRead(in)!=1){
  IN();
  }
  if(digitalRead(out)!=1){
    OUT();
   }

 
        //Read data and store it to variables hum and temp
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    lcd.setCursor(1,1);
    lcd.print(" Temp: ");
    lcd.setCursor(7, 1);
    lcd.print(temp);   
    delay(2000);
    delay(2000); //Delay 2 sec.
    if (temp>=35){
      digitalWrite(5, 255);      // Almost any value can be used except 0 and 255
      Serial.println("Buzz");                     // experiment to get the best tone
      delay(5000);          // wait for a delayms ms
      digitalWrite(5, 0);       // 0 turns it off
         }
}
