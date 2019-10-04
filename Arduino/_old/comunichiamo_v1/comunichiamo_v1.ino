const int PUSH= 4;
const int CHA = 3;
const int CHB = 2;



// SERIAL STUFF ////////////////////////////////////////////////////////
bool bSendSerial = true;

// ENCODER STUFF /////////////////////////////////////////////////////// 
// Encoder code is inspired by the work of eran.io
// http://eran.io/rotary-encoder-based-cooking-timer/
// Here's code: https://github.com/pavius/rotary-encoder-timer

#include "eran_encoder.h"

EranEncoder encoder;
int eValue, ePrevValue;

void cwStep(void){
  if( bSendSerial) Serial.write( '0' );
}

void ccwStep(void){
  if( bSendSerial) Serial.write( '1' );
}



// BUTTON STUFF ////////////////////////////////////////////////////////
// (ButtonDebounce Library is written and maintained 
// by Maykon L. Capellari <maykonluiscapellari@gmail.com>
// https://github.com/maykon/ButtonDebounce

#include "ButtonDebounce.h"

ButtonDebounce push(2, 50);

void pushChanged(int state){
  if( !state )
  {
    if( bSendSerial) Serial.write('c');
  }
  else
  {
     if( bSendSerial) Serial.write('r');
  }
}
/*
ButtonDebounce shuffle(8, 50);

void shuffleChanged(int state){
  if( !state )
  {
    if( bSendSerial) Serial.write('r');
  }
}

ButtonDebounce back(9, 50);

void backChanged(int state){
  if( !state )
  {
    if( bSendSerial) Serial.write('l');
  }
}
*/

// SETUP ///////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  encoder.init(7,3);
  encoder.setCallbackCW(cwStep);
  encoder.setCallbackCCW(ccwStep);

  push.setCallback(pushChanged);
  //shuffle.setCallback(shuffleChanged);
  //back.setCallback(backChanged);
}

// LOOP ////////////////////////////////////////////////////////////////
void loop() 
{
  //getSerialData();
  
  encoder.update();
  push.update();
  //shuffle.update();
  //back.update();

  delay(5); 
}

// SERIAL UTILITY FUNC /////////////////////////////////////////////////
/*
void getSerialData()
{
  if(Serial.available()) {
    char user_input = Serial.read(); // Read user input and trigger appropriate function
      
    if (user_input =='1')  {
       bSendSerial = true;
    }
    else if(user_input =='0')  {
      bSendSerial = false;
    }
  }
}
*/
