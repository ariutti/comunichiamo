/*
  Animator AR
  Nicola Ariutti
  2018/09/25
*/

#ifndef ANIMATOR_AR_H
#define ANIMATOR_AR_H

#include "Arduino.h"

class Animator_AR
{
public:
  Animator_AR() {};
  ~Animator_AR() {};

  void init( int t_1, int t_2 );
  void update();
  void trigger();
  float getY();
 

private:

  // note that time variables must be 
  // long in order to store great values.
  int t1, t2;  // t attack, t release
  long dt;      // delta time
  long st;      // start time

  // states
  enum {
    QUIET = -1,
    ATTACK,
    RELEASE
  } state;

  float y;
};
#endif
