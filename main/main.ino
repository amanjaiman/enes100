#include "Enes100.h"

int dest_x;
int dest_y;

void setup() {
  while (!enes.retrieveDestination()) {
    enes.println("Unable to retrieve location");
  }
  dest_x = enes.destination.x;
  dest_y = enes.destination.y;
}

void loop() {
  int my_x;
  int my_y;
  float my_theta;
  // Update OSV location
  if (enes.updateLocation()) {
    my_x = enes.location.x;
    my_y = enes.location.y;
    my_theta = enes.location.theta;
  } else {
    enes.println("Error: Couldn't update location")
  }
}
