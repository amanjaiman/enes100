# ENES100 Fall 2018
This is the Arduino C code for our OSV.

## Background
Team Ironsight was tasked with constructing an Over Sand Vehicle (OSV) that can autonomously traverse through a remote island in the Pacific Ocean in order to accomplish an assigned mission related to the aircraft debris found on the island. Our mission is to travel to the aircraft debris to within 250 mm in order to transmit the material present. We also need to acquire the debris by lifting it off the sand and determine the mass of the debris found to within 20 g.

## OSV Structure
Our OSV has a 250 mm by 290 mm chassis, built out of plywood, and rests on four Dague wheels that allow the OSV to have enough traction to not slip when travelling on sand. It is powered using the back two wheels, each of which are connected to a 6V motor. The OSV also has ultrasonic sensors on the front and sides of the chassis for obstacle detection, as well as the inductance proximity sensor on the front to identify the material. In order to pick up and weigh the debris, the OSV has a hook attached to a lever arm that can latch onto the debris and lift it off the ground.

## Navigation and Mission
The OSV is designed to communicate with a overhead camera through a RF transceiver and Arduino code. To begin, our OSV will be placed in a random orientation in a random location on the left side of the island. From there, the OSV will have to figure out the right way to move and travel to the mission site, which can be done by the coordinates and angle given by the RF transceiver. Along the way, it will hit a rocky terrain area, which will be tough to get over if the OSV does not have the appropriate wheels and power. Once the OSV is past the rocky terrain, it will encounter obstacles on its way to the mission site. Using the onboard ultrasonic sensors, our OSV can detect these obstacles and travel around them, then continuing its path. At the mission site, the OSV will need to find the debris, so it will move around until it senses another object in the mission site, since there cannot be any obstacles in the mission site. Once the OSV reaches the material, it will determine what the material is using the inductance proximity sensor, which generates a magnetic field to detect any metallic object. The sensor will detect the two different types of metals (copper or steel) based on the magnetic permeability.

