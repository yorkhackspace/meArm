/* meArm library York Hack Space May 2014
 * A simple control library for Phenoptix' meArm
 * Usage:
 *   meArm arm;
 *   arm.begin(1, 10, 9, 6);
 *   arm.openGripper();
 *   arm.gotoPoint(-80, 100, 140);
 *   arm.closeGripper();
 *   arm.gotoPoint(70, 200, 10);
 *   arm.openGripper();
 */
#include <Arduino.h>
#include "ik.h"
#include "meArm.h"
#include <Servo.h>
bool setup_servo (ServoInfo& svo, const int n_min, const int n_max,
                  const float a_min, const float a_max)
{
    float n_range = n_max - n_min;//pwm角度范围 
    float a_range = a_max - a_min;//范围 弧度

    // Must have a non-zero angle range
    if(a_range == 0) return false;

    // Calculate gain and zero
    svo.gain = n_range / a_range;//计算增益 k
    svo.zero = n_min - svo.gain * a_min;//零角度 b

    // Set limits
    svo.n_min = n_min;//设置范围
    svo.n_max = n_max;

    return true;
}
//弧度到角度
int angle2pwm (const ServoInfo& svo, const float angle)
{
    float pwm = 0.5f + svo.zero + svo.gain * angle;//为什么要加0.5f  换算成pwm角度
    return int(pwm);
}

//Full constructor with calibration data
meArm::meArm(int sweepMinBase, int sweepMaxBase, float angleMinBase, float angleMaxBase,
          int sweepMinShoulder, int sweepMaxShoulder, float angleMinShoulder, float angleMaxShoulder,
          int sweepMinElbow, int sweepMaxElbow, float angleMinElbow, float angleMaxElbow,
          int sweepMinGripper, int sweepMaxGripper, float angleMinGripper, float angleMaxGripper) {
  //calroutine();
  setup_servo(_svoBase, sweepMinBase, sweepMaxBase, angleMinBase, angleMaxBase);
  setup_servo(_svoShoulder, sweepMinShoulder, sweepMaxShoulder, angleMinShoulder, angleMaxShoulder);
  setup_servo(_svoElbow, sweepMinElbow, sweepMaxElbow, angleMinElbow, angleMaxElbow);
  setup_servo(_svoGripper, sweepMinGripper, sweepMaxGripper, angleMinGripper, angleMaxGripper);
}

void meArm::begin(int pinBase, int pinShoulder, int pinElbow, int pinGripper) {
  _pinBase = pinBase;
  _pinShoulder = pinShoulder;
  _pinElbow = pinElbow;
  _pinGripper = pinGripper;
  _base.attach(_pinBase);
  _shoulder.attach(_pinShoulder);
  _elbow.attach(_pinElbow);
  _gripper.attach(_pinGripper);

  //goDirectlyTo(0, 100, 50);
  goDirectlyToCylinder(0, 100, 50);//初始位置
  openGripper();
}

void meArm::end() {
  _base.detach();
  _shoulder.detach();
  _elbow.detach();
  _gripper.detach();
}

//Set servos to reach a certain point directly without caring how we get there 
void meArm::goDirectlyTo(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  if (solve(x, y, z, radBase, radShoulder, radElbow)) {
    _base.write(angle2pwm(_svoBase,radBase));
    _shoulder.write(angle2pwm(_svoShoulder,radShoulder));
    _elbow.write(angle2pwm(_svoElbow,radElbow));
    _x = x; _y = y; _z = z;
  }    
}

//Travel smoothly from current point to another point
void meArm::gotoPoint(float x, float y, float z) {
  //Starting points - current pos
  float x0 = _x; 
  float y0 = _y; 
  float z0 = _z;
  float dist = sqrt((x0-x)*(x0-x)+(y0-y)*(y0-y)+(z0-z)*(z0-z));
  int step = 10;
  for (int i = 0; i<dist; i+= step) {
    goDirectlyTo(x0 + (x-x0)*i/dist, y0 + (y-y0) * i/dist, z0 + (z-z0) * i/dist);
    delay(50);
  }
  goDirectlyTo(x, y, z);
  delay(50);
}

//Get x and y from theta and r
void meArm::polarToCartesian(float theta, float r, float& x, float& y){
    _r = r;
    _t = theta;
    x = r*sin(theta);
    y = r*cos(theta);
}

//Same as above but for cylindrical polar coodrinates
void meArm::gotoPointCylinder(float theta, float r, float z){
    float x, y;
    polarToCartesian(theta, r, x, y);
    gotoPoint(x,y,z);
}

void meArm::goDirectlyToCylinder(float theta, float r, float z){
    float x, y;
    polarToCartesian(theta, r, x, y);
    goDirectlyTo(x,y,z);
}

//Check to see if possible
bool meArm::isReachable(float x, float y, float z) {
  float radBase,radShoulder,radElbow;
  return (solve(x, y, z, radBase, radShoulder, radElbow));
}

//Grab something
void meArm::openGripper() {
  _gripper.write(angle2pwm(_svoGripper,pi/2));
  delay(300);
}

//Let go of something
void meArm::closeGripper() {
  _gripper.write(angle2pwm(_svoGripper,0));
  delay(300);
}

//Current x, y and z
float meArm::getX() {
  return _x;
}
float meArm::getY() {
  return _y;
}
float meArm::getZ() {
  return _z;
}


float meArm::getR() {
  return _r;
}
float meArm::getTheta() {
  return _t;
}
