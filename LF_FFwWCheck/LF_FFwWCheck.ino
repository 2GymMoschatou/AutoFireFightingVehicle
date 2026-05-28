/*
 2GM Roboteam
 Firefighting vechicle
*/
//Including libraries
#include <Servo.h>
//Fire fighting settings
Servo myservo; //Creating servo
const int flamePin = 2; //flame sensor pin
const int relayPin = 4; //relay pin
const int beeperPin = 13; //beeper pin
const int waterPin = A3; //water level sensor pin
// variables
int flameState = 0; //flame status
int pos = 0; //servo initial position
int waterLevel = 0; //water level value
const int waterMin = 150; //minimum water tank level
const int waterMax = 300; //maximum water tank level
const int scanInterval=1000; //time between scans
unsigned long lastScanTime=0; //last scan time
//Line follower settings
//Motor A (right)
const int enA = 6;
const int motorPin1  = 9;
const int motorPin2  = 8;
//Motor B (left)
const int enB = 5;
const int motorPin3  = 7;
const int motorPin4  = 10;
//Ultrasonic Sensor
const int echoPin = 11;
const int trigPin = 12;
long duration, distance, cm;
//IRsensors
const int leftIR = A2;
const int middleIR = A1;
const int rightIR = A0;
int leftID, middleID, rightID;

void setup() {
  //Set pins for Fire fighting
  pinMode(flamePin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(beeperPin, OUTPUT);
  myservo.attach(3);
  //Set pins for motors
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  analogWrite(enA,200);
  analogWrite(enB,200);
  //Set pins for ultrasonic sensor
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  //Set pins as inputs for IR sensors
  pinMode(leftIR, INPUT);
  pinMode(middleIR, INPUT);
  pinMode(rightIR, INPUT);
}

void loop() {
  waterLevel = analogRead(waterPin);    
  cm = getDistance();
  if (cm <= 15){
    stop();
  }
  else {
    move();
  }
  if (millis()-lastScanTime>=scanInterval){
    stop();
    fireDetect();
    lastScanTime=millis();
  }
}

void fireDetect(){
  for (pos = 0; pos <= 180; pos += 15) {
    myservo.write(pos);
    delay(500);
    // read the flame state:
    flameState = digitalRead(flamePin);
    
    while (flameState == 1 && waterLevel > waterMin) {
      fireFight();
    }
  }
}

void fireFight(){
  // turn pump & beeper on:
  digitalWrite(relayPin, HIGH);
  tone(beeperPin, 1000, 1000);
  delay(1000);
  digitalWrite(relayPin, LOW);
  flameState = digitalRead(flamePin);
  waterLevel = analogRead(waterPin);
}

void move(){
  leftID = digitalRead(leftIR);
  middleID = digitalRead(middleIR);
  rightID = digitalRead(rightIR);
  
  if (leftID==HIGH && middleID==HIGH && rightID==HIGH){
    stop();
    checkWater();
  }
  else if (leftID==LOW && middleID==HIGH && rightID==LOW){
    forward();
  }
  else if (leftID==HIGH && middleID==HIGH && rightID==LOW){
    turnLeft();
  }
  else if (leftID==LOW && middleID==HIGH && rightID==HIGH){
    turnRight();
  }
  else if (leftID==HIGH && middleID==LOW && rightID==LOW){
    sharpTurnLeft();
  }
  else if (leftID==LOW && middleID==LOW && rightID==HIGH){
    sharpTurnRight();
  }
  else {
    backward();
  }
}

void checkWater(){
  waterLevel = analogRead(waterPin);
  if (waterLevel < waterMax){
    tone(beeperPin, 500, 300);
    stop();
  }
  if (waterLevel >= waterMax){
    go();
  }  
}
  
void forward(){
  // This code will move platform forward for 1 sec.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(10);  
}

void backward(){
  // This code will move platform backward  for 1 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(5);  
}

void turnLeft(){
  //This code  will turn platform left for 0.5 sec.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(10);  
}

void turnRight(){
  //This code  will turn platform right for 0.5 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(10);
}

void sharpTurnLeft(){
  //This code  will sharp turn platform left for 0.5 sec.
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  delay(10);  
}

void sharpTurnRight(){
  //This code  will sharp turn platform left for 0.5 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(10);
}

void stop(){
  //This code will stop platform for 2 sec.
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  delay(2000);
}

void go(){
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  delay(200);
}

long getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;
}
