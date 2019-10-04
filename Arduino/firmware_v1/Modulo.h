/*
  Modulo
  Nicola Ariutti
  2018/09/25
*/

#ifndef MODULO_H
#define MODULO_H

#include "Arduino.h"

class Modulo
{
  private:
  public:
    Modulo() {};
    ~Modulo() {};

    void init(float _freq, float _phase);
    void update();
    void changeFreq(float _freq);
    float getY();
};

#endif
