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

void moveBackward(double time) {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(-255);
  delay(time);
  tank.turnOffMotors();
}

void turnLeft(double time) {
  tank.setLeftMotorPWM(-255);
  tank.setRightMotorPWM(255);
  delay(time);
  tank.turnOffMotors();
}

void turnRight(double time) {
  tank.setLeftMotorPWM(255);
  tank.setRightMotorPWM(-255);
  delay(time);
  tank.turnOffMotors();
}

double getOriginAngleToMission() {
  return atan(abs(my_y-dest_y)/abs(my_x-dest_x));
}

void loop() {
  updateOSVLocation();

  double missionAngle = getOriginAngleToMission();
  if (my_y > dest_y) {
    missionAngle = -1*missionAngle;
  }
  
  while (my_theta > missionAngle+0.05 or my_theta < missionAngle-0.05) {
    turnLeft(50);
    updateOSVLocation();
  }
  bool s = true;
  moveForward();
  while (my_x < dest_x) {
    updateOSVLocation();
  }
  tank.turnOffMotors();
  
  enes.println("Stopped...");
  exit(0);
}
