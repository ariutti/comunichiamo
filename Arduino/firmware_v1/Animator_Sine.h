/*
  Animator Sine
  Nicola Ariutti
  2018/09/25
*/

#ifndef ANIMATOR_SINE_H
#define ANIMATOR_SINE_H

#include "Arduino.h"

class Animator_Sine
{
  private:
    float freq;
    float phase;
    float t, t0, dt;
    float y;
  public:
    Animator_Sine() {};
    ~Animator_Sine() {};

    void init(float _freq, float _phase);
    void update();
    void changeFreq(float _freq);
    float getY();
};

#endif
