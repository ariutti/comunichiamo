#include "Animator_AR.h"


// Set attack and release time in millisecond
// default are attack time (t_1) = 10ms and
// release time (t_2) = 500ms
void Animator_AR::init( int t_1, int t_2 ) {
  t1 = t_1>0.0?t_1:10;
  t2 = t_2>0.0?t_2:500;
  state = QUIET;
}

void Animator_AR::update()
{
  if(state == ATTACK)
  {
    dt = millis() - st;
    // TODO: insert a check here in order to prevent
    // cases where 'dt' become grater than t1.
    // Update st accordingly.
    //if( dt < 0 )
    //  Serial.println("ERROR");
    y =  (1.0*dt)/t1;
    if(y > 1.0)
    {
      y = 1.0;
      state = RELEASE;
    }
  }
  else if( state == RELEASE )
  {
    dt = millis() - st;
    // TODO: insert a check here in order to prevent
    // cases where 'dt' become grater than t2.
    // Update st accordingly.
    y =  1.0 - ( (1.0*dt)/t2 );
    if(y < 0.0)
    {
      y = 0.0;
      state = QUIET;
    }
  }
}

void Animator_AR::trigger()
{
  st = millis();
  state = ATTACK;
}

float Animator_AR::getY()
{
  return y;
}

