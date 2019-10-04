#include "Animator_Sine.h"

void Animator_Sine::init( float _freq, float _phase )
{
  freq = _freq>0.0?_freq:1;
  phase = abs(int(_phase))%(int(2*PI));
  //phase = 0.0;
  // At the beginning 't0' and 't' are equal.
  t0 = t = (millis() * 0.001);
  dt = t - t0;
}

void Animator_Sine::update()
{
  t = (millis() * 0.001);
  dt = t - t0;
  y = sin( phase + 2*PI*freq*dt );
  y = (y+1)*0.5;
}

void Animator_Sine::changeFreq( float _freq )
{
  // calculate the phase for the
  // upcoming sinusoid
  t = (millis() * 0.001);
  dt = t - t0;
  phase = phase + 2*PI*freq*dt;
  phase = int(phase) % int(2*PI);
  // define the new 't0' which is the
  // new reference for counting time
  t0 = t;
  // Finally, set the new frequency
  freq = _freq;
}

float Animator_Sine::getY() 
{
  return y;
}

