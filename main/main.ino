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

void avoidXObstacle() {
  updateOSVLocation();
  while (my_theta > 1.60 or my_theta < 1.54) {
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
    if (enes.readDistanceSensor(1) < .175 or enes.readDistanceSensor(0) < .175 or enes.readDistanceSensor(2) < .175) {
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

  moveForward();
  while (abs(my_y-dest_y) > 0.05) {
    updateOSVLocation();
    if (enes.readDistanceSensor(1) < .175 or enes.readDistanceSensor(0) < .175 or enes.readDistanceSensor(2) < .175) {
      tank.turnOffMotors();
      avoidYObstacle();
      moveForward();
    }
  }
  tank.turnOffMotors();
  
  enes.println("Stopped...");
  exit(0);
}
