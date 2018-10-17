#include "Enes100Simulation.h"
#include "DFRTankSimulation.h"

//#include "Enes100.h"
//Enes100 enes("Ironsight", DEBRIS, 3, 8, 9);

Enes100Simulation enes;
DFRTankSimulation tank;

int dest_x;
int dest_y;
int my_x;
int my_y;
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

void moveForward() {
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(255);
}

void moveBackward() {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
}

void turnLeft() {
  tank.setLeftMotorPWM(-100);
  tank.setRightMotorPWM(100);
}

void turnRight() {
  tank.setLeftMotorPWM(100);
  tank.setRightMotorPWM(-100);
}

void loop() {

  //updateOSVLocation();
  moveForward();

}