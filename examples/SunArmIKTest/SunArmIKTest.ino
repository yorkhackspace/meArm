/* SunArm IK test - Sunny 2020 WTServo16
 * Test applying Nick Moriarty's Inverse Kinematics solver
 * to SunArm robot arm, to walk a specified path.
 *尽量减少修改量
 * Pins:
 * WTServo16    Base   Shoulder  Elbow    Gripper
 *    GND    Brown     Brown   Brown     Brown
 *     5V      Red       Red     Red       Red
 *     M0    Yellow
 *     M1             Yellow
 *     M2                     Yellow
 *     M3                               Yellow
 */
#include "SunArm.h"

// WTServo16 pin
const int basePin = 0;
const int shoulderPin = 1;
const int elbowPin = 2;
const int gripperPin = 3;

SunArm arm;

void setup() {
  arm.begin(basePin, shoulderPin, elbowPin, gripperPin);
}

void loop() {
  //Clap - so it's obvious we're at this part of the routine
  arm.openGripper();
  arm.closeGripper();
  arm.openGripper();
  arm.closeGripper();
  arm.openGripper();
  delay(500);
  //Go up and left to grab something
  arm.gotoPoint(-80,100,140); 
  arm.closeGripper();
  //Go down, forward and right to drop it
  arm.gotoPoint(70,200,10);
  arm.openGripper();
  //Back to start position
  arm.gotoPoint(0,100,50);
  delay(2000);
}
