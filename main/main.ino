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

void avoidXObstacle() {
  updateOSVLocation();
  while (my_theta > 1.62 or my_theta < 1.52) {
    updateOSVLocation();
    turnLeft();
  }
  tank.turnOffMotors();
}

void avoidYObstacle() {
  updateOSVLocation();
  
}

void loop() {
  updateOSVLocation();

  /*double missionAngle = getOriginAngleToMission();
  if (my_y > dest_y) {
    missionAngle = -1*missionAngle;
  }
  
  while (my_theta > missionAngle+0.05 or my_theta < missionAngle-0.05) {
    turnLeft(50);
    updateOSVLocation();
  }*/

  turnLeft();
  while (my_theta > 0.05 or my_theta < -0.05) {
    updateOSVLocation();
  }
  tank.turnOffMotors();
  
  moveForward();
  while (my_x < dest_x) {
    updateOSVLocation();
    if (enes.readDistanceSensor(1) < .3) {
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
  turnOffMotors();

  moveForward();
  while (abs(my_y-dest_y) > 0.05) {
    updateOSVLocation();
    if (enes.readDistanceSensor(1) < .3) {
      tank.turnOffMotors();
      avoidYObstacle();
      moveForward();
    }
  }
  tank.turnOffMotors();
  
  enes.println("Stopped...");
  exit(0);
}
