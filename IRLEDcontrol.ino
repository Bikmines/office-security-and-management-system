
//https://www.youtube.com/user/greatscottlab

#include <IRremote.h>
#include <Servo.h> 
#define in 8

Servo myservo;
int pos = 0;

int RELAY_PIN = 2;

int bright;
int before;
int out=13; //connect your LED to pin 9 
int RECV_PIN = 3; //data out of IR receiver connects to pin 11

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn(); // start the receiver
  before=0; //LED is turned off
  bright=255; //brightness value is at maximum (255)
  pinMode(in, INPUT);
  myservo.attach(6);
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(out,OUTPUT);
}

void loop() {
  Serial.println(digitalRead(in));
  if(digitalRead(in)!=1){
    myservo.write(60);
    delay(200);
    myservo.write(0);
    }
  
if (irrecv.decode(&results)) {
  
  if (results.value==0xFFB24D){ //Code to turn the LED ON/OFF
    if(before==0){ // if the LED was turned off, then we turn it on 
     digitalWrite(RELAY_PIN, HIGH);
     digitalWrite(out, HIGH);
      before=1; //LED is now turned on
      Serial.println("on");
    }
    else{
      digitalWrite(RELAY_PIN,LOW); //if the LED was turned on, then we turn it off
      before=0;
      bright=255;
      digitalWrite(out, LOW);
      Serial.println("off"); 
    }} 
  irrecv.resume();
}
}







