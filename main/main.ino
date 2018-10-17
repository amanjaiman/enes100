#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

//#include "Enes100.h"
//Enes100 enes("Ironsight", DEBRIS, 3, 8, 9);

Enes100Simulation enes;
DFRTankSimulation tank;

float dest_x;
float dest_y;
float my_x;
float my_y;
float my_theta;

void setup() {
  tank.init();
  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
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

void moveForward(int time) {
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
  delay(time);
  tank.turnOffMotors();
}

void moveBackward(float time) {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
  delay(time);
  tank.turnOffMotors();
}

void turnLeft(float time) {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(time);
  tank.turnOffMotors();
}

void turnRight(float time) {
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
  delay(time);
  tank.turnOffMotors();
}

void loop() {
  updateOSVLocation();
  enes.println(my_x);
  enes.println(my_y);
  enes.println(my_theta);
  enes.println(dest_x);
  enes.println(dest_y);
  exit(0);
}
