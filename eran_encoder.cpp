#include "eran_encoder.h"

void EranEncoder::init(int _cha, int _chb)
{
  CHA = _cha;
  CHB = _chb;
}

void EranEncoder::update()
{
  statusChA = ((statusChA<<1) | digitalRead(CHA)) & 0x07;
  statusChB = ((statusChB<<1) | digitalRead(CHB)) & 0x07;

  // debounce CH A
  if( statusChA == searchPatternA )
  {
    // change search pattern
    searchPatternA = (~searchPatternA) & 0x07;
    // append 0 to resutl
    transitionEvent = statusChA; // 0 or 7
  }

  // debounce CH B
  if( statusChB == searchPatternB )
  {
    // change search pattern
    searchPatternB = (~searchPatternB) & 0x07;
    // append 1 to result
    transitionEvent = (statusChB | (1<<3)); // 8 or 15
  }
 
  // Check pattern (if something has happend
  // the transition event is now 7, 15, 0 or 8.
  if( transitionEvent != NO_TRANSITION_EVENT )
  {
    if( state == STATE_IDLE )
    {
      // first value of a CW transition?
      if(transitionEvent == cwTransitions[0])
      {
        // yes! so we enter the corresponding state
        state = STATE_DETECTING_CW;
        // prepare for the next transition;
        transitionEventIndex++;
      }
      // first value of a CCW transition?
      else if(transitionEvent == ccwTransitions[0])
      {
        // yes! so we enter the corresponding state
        state = STATE_DETECTING_CCW;
        // prepare for the next transition;
        transitionEventIndex++;
      }
    }
    else if( state == STATE_DETECTING_CW )
    {
      if(transitionEvent == cwTransitions[transitionEventIndex])
      {
        // increment
        transitionEventIndex++;

        if(transitionEventIndex == 2)
        {
          changeCounts++;
          if(this->callback_cw) this->callback_cw();
        }
        else if(transitionEventIndex == 4) // have we finished?
        {
          // increment count
          changeCounts++;
          if(this->callback_cw) this->callback_cw();
          //back to idle
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }
        
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
        transitionEventIndex = 0;
      }
    }
    else if( state == STATE_DETECTING_CCW )
    {
      if(transitionEvent == ccwTransitions[transitionEventIndex])
      {
        // increment
        transitionEventIndex++;

        if(transitionEventIndex == 2)
        {
          changeCounts--;
          if(this->callback_ccw) this->callback_ccw();
        }
        else if(transitionEventIndex == 4) // have we finished?
        {
          // decrement count
          changeCounts--;
          if(this->callback_ccw) this->callback_ccw();
          //back to idle
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }

      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
        transitionEventIndex = 0;
      } 
    }
    // we have already processed the transition occured so
    // we must zero out the transition.
    transitionEvent = NO_TRANSITION_EVENT;
  }
}

void EranEncoder::resetCounter()
{
  changeCounts = 0;
}


int EranEncoder::getCounter()
{
  return changeCounts;
}


void EranEncoder::setCallbackCW(ENCODER_CALLBACK_CW){
  this->callback_cw = callback_cw;
}


void EranEncoder::setCallbackCCW(ENCODER_CALLBACK_CCW){
  this->callback_ccw = callback_ccw;
}


