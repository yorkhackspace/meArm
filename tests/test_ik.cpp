#include "ik.h"
#include <cstdio>

const float pi=3.14159265359;

struct ServoInfo {
    int n_min, n_max;   // PWM 'soft' limits - should be just within range
    float gain;         // PWM per radian
    float zero;         // Theoretical PWM for zero angle
};

ServoInfo svoB, svoS, svoE;

bool setup_servo (ServoInfo& svo, const int n_min, const int n_max,
                  const float a_min, const float a_max)
{
    float n_range = n_max - n_min;
    float a_range = a_max - a_min;

    // Must have a non-zero angle range
    if(a_range == 0) return false;

    // Calculate gain and zero
    svo.gain = n_range / a_range;
    svo.zero = n_min - svo.gain * a_min;

    // Set limits
    svo.n_min = n_min;
    svo.n_max = n_max;

    return true;
}

int angle2pwm (const ServoInfo& svo, const float angle)
{
    float pwm = 0.5f + svo.zero + svo.gain * angle;
    return int(pwm);
}

void test(float x, float y, float z)
{
  float b,s,e;
  printf("posn=(%.1f, %.1f, %.1f); ", x,y,z);

  if( solve(x,y,z,b,s,e) )
  {
    printf("b=%.3f, s=%.3f, e=%.3f; ", b,s,e);

    int ib = angle2pwm(svoB, b);
    int is = angle2pwm(svoS, s);
    int ie = angle2pwm(svoE, e);

    printf("ib=%d, is=%d, ie=%d", ib, is, ie);
  }
  else
    printf("Not solvable");
  printf("\n");
}

int main()
{
  // Calibration as reported in issue
  setup_servo(svoB, 173,24, -pi/2, pi/2  );
  setup_servo(svoS, 178,50, pi/4,  3*pi/4);
  setup_servo(svoE, 178,73, 0,     -pi/4 );

  test(0, 190, 200);
  test(0, 190, 120);

  return 0;
}
