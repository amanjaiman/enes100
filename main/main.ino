// https://playground.arduino.cc/Code/PIDLibraryAdaptiveTuningsExample

#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

//#include "Enes100.h"
//Enes100 enes("Ironsight", DEBRIS, 3, 8, 9);

Enes100Simulation enes;
DFRTankSimulation tank;

double dest_x;
double dest_y;
double my_x;
double my_y;
double my_theta;

void setup() {
  tank.init();
  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
    delay(300);
  }
  dest_x = enes.destination.x;
  dest_y = enes.destination.y;
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
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
}

void moveBackward() {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
}

void turnLeft() {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
}

void turnRight() {
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
}

int turn() { // 0 for left, 1 for right
  // Find out which way to move:
  if (enes.readDistanceSensor(10) < .3) { // Cannot move forward because to close to top
    return 1;
  }
  else {
    if (my_y > dest_y) {
      if (enes.readDistanceSensor(4) < .3) { // Cannot move backwards because to close to bottom
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

void avoidXObstacle() { // WHAT IF OBSTACLE IS RIGHT ABOVE/BELOW MISSION SITE
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
  tank.turnOffMotors();

  // Clear obstacle side
  moveForward();
  enes.println(sensor);
  while (enes.readDistanceSensor(sensor) < 0.5) {
    enes.println("Clearing obstacle...");
    updateOSVLocation();
  }
  delay(1000);
  tank.turnOffMotors();

  if (turnDirection == 0) {
    turnRight();
  }
  else {
    turnLeft();
  }
  while (my_theta > 0.05 or my_theta < -0.05) {
    updateOSVLocation();
  }
  tank.turnOffMotors();
  
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
  tank.turnOffMotors();

  // Clear obstacle side
  moveForward();
  enes.println(sensor);
  while (enes.readDistanceSensor(sensor) < 0.5) {
    enes.println("Clearing obstacle...");
    updateOSVLocation();
  }
  delay(1000);
  tank.turnOffMotors();

  if (turnDirection == 0) {
    turnRight();
  }
  else {
    turnLeft();
  }
  while (my_theta > 1.6 or my_theta < 1.53) {
    updateOSVLocation();
  }
  tank.turnOffMotors();
}

void loop() {
  updateOSVLocation();
  enes.print("X: ");
  enes.println(my_x);
  enes.print("Y: ");
  enes.println(my_y);
  enes.print("Theta: ");
  enes.println(my_theta);

  while (my_theta > 0.05 or my_theta < -0.05) {
    updateOSVLocation();
    turnLeft();
  }
  tank.turnOffMotors();
  
  moveForward();
  while (my_x < dest_x) {
    updateOSVLocation();
    if (enes.readDistanceSensor(0) < .175 or enes.readDistanceSensor(2) < .175) {
      tank.turnOffMotors();
      avoidXObstacle();
      moveForward();
    }
  }
  tank.turnOffMotors();
  updateOSVLocation();

  if (my_y > dest_y) {
    turnRight();
    while (my_theta > -1.57) {
      updateOSVLocation();
    }
  }
  else {
    turnLeft();
    while (my_theta < 1.57) {
      updateOSVLocation();
    }
  }
  tank.turnOffMotors();

  updateOSVLocation();
  moveForward();
  while (abs(my_y-dest_y) > 0.05) {
    updateOSVLocation();
    if (enes.readDistanceSensor(0) < .175 or enes.readDistanceSensor(2) < .175) {
      tank.turnOffMotors();
      avoidYObstacle();
      moveForward();
    }
  }
  tank.turnOffMotors();
  
  enes.println("Reached...");
  exit(0);
}
