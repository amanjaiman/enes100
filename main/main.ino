#include <Tlc5940.h>
#include "Enes100.h"
Enes100 enes("Ironsight", DEBRIS, 3, 12, 11);

int leftMotor1 = 10;
int leftMotor2 = 12;
int rightMotor1 = 11;
int rightMotor2 = 13;

int slideRailMotor1 = 1;
int slideRailMotor2 = 9;

int sensor1trig = 3;
int sensor1echo = 2; // Front left
int sensor2trig = 3;
int sensor2echo = 4; // Front right
int sensor3trig = 3;
int sensor3echo = 7; // Right sensor
int sensor4trig = 3;
int sensor4echo = 8; // Left sensor

float dest_x;
float dest_y;
float my_x;
float my_y;
float my_theta;

void setup() {
  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
    delay(300);
  }
  dest_x = enes.destination.x;
  dest_y = enes.destination.y;

  //Tlc.init();
  pinMode(leftMotor1, OUTPUT);
  pinMode(leftMotor2, OUTPUT);
  pinMode(rightMotor1, OUTPUT);
  pinMode(rightMotor2, OUTPUT);
  
  pinMode(sensor1trig, OUTPUT);
  pinMode(sensor1echo, INPUT);
  pinMode(sensor2trig, OUTPUT);
  pinMode(sensor2echo, INPUT);
  pinMode(sensor3trig, OUTPUT);
  pinMode(sensor3echo, INPUT);
  pinMode(sensor4trig, OUTPUT);
  pinMode(sensor4echo, INPUT);
}

void updateOSVLocation() {
  while(!enes.updateLocation()) {
    enes.println("Error: Could not update location");
  }
  my_x = enes.location.x;
  my_y = enes.location.y;
  my_theta = enes.location.theta;
}

void moveForward() {
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
  Tlc.update();
}

void moveBackward() {
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(rightMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
}

void turnLeft() {
  digitalWrite(leftMotor1, HIGH);
  digitalWrite(leftMotor2, LOW);
  digitalWrite(rightMotor2, HIGH);
  digitalWrite(rightMotor1, LOW);
}

void turnRight() {
  digitalWrite(leftMotor2, HIGH);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(rightMotor1, HIGH);
  digitalWrite(rightMotor2, LOW);
}

void stopAllMotors() {
  digitalWrite(leftMotor2, LOW);
  digitalWrite(leftMotor1, LOW);
  digitalWrite(rightMotor1, LOW);
  digitalWrite(rightMotor2, LOW);
}

void moveSlideRailForward() {
  Tlc.set(slideRailMotor1, 4095);
  Tlc.set(slideRailMotor2, 0);
}

void moveSlideRailBackward() {
  Tlc.set(slideRailMotor1, 0);
  Tlc.set(slideRailMotor2, 4095);
}

//For these two methods replace with the actual pin values
int getTrigPin(int sensor){
  if(sensor ==1){
    return sensor1trig;
  }else if(sensor ==2){
    return sensor2trig;
  }else if(sensor ==3){
    return sensor3trig;
  }else{
    return sensor4trig;
  }
}

int getEchoPin(int sensor){
  if(sensor ==1){
    return sensor1echo;
  }else if(sensor ==2){
    return sensor2echo;
  }else if(sensor ==3){
    return sensor3echo;
  }else{
    return sensor4echo;
  }
}

int readDistanceSensor(int sensor){
  long duration;
  digitalWrite(getTrigPin(sensor), LOW);
  delayMicroseconds(2);
  
  digitalWrite(getTrigPin(sensor), HIGH);
  delayMicroseconds(10);
  digitalWrite(getTrigPin(sensor), LOW);

  duration = pulseIn(getEchoPin(sensor), HIGH);
  int distance= duration*0.034/2;

  return distance;
}

int turn() { // 0 for left, 1 for right
  // Find out which way to move:
  if (readDistanceSensor(10) < .3) { // Cannot move forward because to close to top
    return 1;
  }
  else {
    if (my_y > dest_y) {
      if (readDistanceSensor(4) < .3) { // Cannot move backwards because to close to bottom
        return 0;
      }
      else {
        return 1;
      }
    }
    else {
      return 0;
    }
  }
}

void avoidXObstacle() {
  updateOSVLocation();
  delay(500);
  int turnDirection = turn();
  int sensor = 0;
  if (turnDirection == 0) {
    sensor = 10;
    while (my_theta < 1.53 or my_theta > 1.61) {
      updateOSVLocation();
      turnLeft();
    }
  }
  else {
    sensor = 4;
    while (my_theta > -1.53 or my_theta < -1.61) {
      updateOSVLocation();
      turnRight();
    }
  }
  stopAllMotors();

  // Clear obstacle side
  moveForward();
  enes.println(sensor);
  while (readDistanceSensor(sensor) < 0.5) {
    updateOSVLocation();
    if (readDistanceSensor(0) < .175 or readDistanceSensor(2) < .175) {
      stopAllMotors();
      avoidYObstacle();
      moveForward();
    }
  }
  delay(750);
  stopAllMotors();

  if (turnDirection == 0) {
    turnRight();
  }
  else {
    turnLeft();
  }
  while (my_theta > 0.05 or my_theta < -0.05) {
    updateOSVLocation();
  }
  stopAllMotors();
  
}

void avoidYObstacle() {
  updateOSVLocation();
  delay(500);
  int turnDirection = turn();
  int sensor = 0;
  if (turnDirection == 0) {
    sensor = 4;
    while (my_theta < 3.1 or my_theta > -3.1) {
      updateOSVLocation();
      turnLeft();
    }
  }
  else {
    sensor = 10;
    while (my_theta > 0.05 or my_theta < -0.05) {
      updateOSVLocation();
      turnRight();
    }
  }
  stopAllMotors();

  // Clear obstacle side
  moveForward();
  enes.println(sensor);
  while (readDistanceSensor(sensor) < 0.5) {
    updateOSVLocation();
    if (readDistanceSensor(0) < .175 or readDistanceSensor(2) < .175) {
      stopAllMotors();
      avoidXObstacle();
      moveForward();
    }
  }
  delay(750);
  stopAllMotors();

  if (turnDirection == 0) {
    turnRight();
  }
  else {
    turnLeft();
  }
  while (my_theta > 1.6 or my_theta < 1.53) {
    updateOSVLocation();
  }
  stopAllMotors();
}

void loop() {
  updateOSVLocation();
  enes.print("X: ");
  enes.println(my_x);
  enes.print("Y: ");
  enes.println(my_y);
  enes.print("Theta: ");
  enes.println(my_theta);

  while (my_theta > 0.04 or my_theta < -0.04) {
    updateOSVLocation();
    turnLeft();
  }
  stopAllMotors();
  
  /*moveForward();
  while (my_x < dest_x) {
    updateOSVLocation();
    if (readDistanceSensor(1) < .175 or readDistanceSensor(2) < .175) {
      stopAllMotors();
      avoidXObstacle();
      moveForward();
    }
  }
  stopAllMotors();
  updateOSVLocation();

  if (my_y > dest_y) {
    turnRight();
    while (my_theta > -1.56) {
      updateOSVLocation();
    }
  }
  else {
    turnLeft();
    while (my_theta < 1.56) {
      updateOSVLocation();
    }
  }
  stopAllMotors();

  enes.println("Reached X");

  updateOSVLocation();
  moveForward();
  while (abs(my_y-dest_y) > 0.05) {
    updateOSVLocation();
    if (readDistanceSensor(1) < .175 or readDistanceSensor(2) < .175) {
      stopAllMotors();
      avoidYObstacle();
      moveForward();
    }
  }
  stopAllMotors();
  updateOSVLocation();

  enes.println("Reached Y");

  if (my_theta < 0) {
    turnLeft();
  }
  else {
    turnRight();
  }
  while (my_theta > 0.04 or my_theta < -0.04) {
    updateOSVLocation();
  }
  stopAllMotors();
  if (my_x > dest_x) {
    moveBackward();
    while (my_x > dest_x) {
      updateOSVLocation();
    }
  }
  else if (my_x < dest_x) {
    moveForward();
    while (my_x < dest_x) {
      updateOSVLocation();
    }
  }
  stopAllMotors();
  
  enes.navigated();*/
  exit(0);
}
