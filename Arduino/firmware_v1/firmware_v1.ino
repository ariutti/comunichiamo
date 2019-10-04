// Using an Arduino Micro as microcontroller
// using NeoPixel LED strip
// using interal pullups instead of externals physical resistors
// 21 LEDS for each Encoder


/* 
 
 * 1st LED strip VCC    to external 5V power supply
 * 1st LED strip GND    to Arduino GND;
 * 1st LED strip DATA   to Arduino digital pin 8 (through a 470 Ohm resistor);
 
 MODULE 1 CONNECTIONS:
 * 1st encoder CHA pin  to Arduino digital pin 2
 * 1st encoder CHB pin  to Arduino digital pin 3
 * 1st encoder PUSH pin to Arduino digital pin 4


 MODULE 2 CONNECTIONS:
 * 2nd encoder CHA pin to Arduino digital pin 5;
 * 2nd encoder CHB pin to Arduino digital pin 6;
 * 2nd encoder PUSH pin to Arduino digital pin 7;

 NOTE: place a 6,3V 1000 microF capacitor between LEDs GND and VCC rails;
 Place one 10K pullup resistor for each encoder pin;

 SERIAL COMMUNICATION PROTOCOL

 <encoder SX (+1/-1) > | < push SX> | <encoder DX (+1/-1) > | < push DX> | <a capo>
*/

// //////////////
// PIN DEFINITION
// //////////////

#define LEDSTRIP_DATA_PIN         9

// module 1

#define M1_ENC_CHA_PIN            4
#define M1_ENC_CHB_PIN            3
#define M1_ENC_PUSH_PIN           2

// /////////////
// STATE MACHINE
// /////////////

enum m_statuses{
  STANDBY=0,
  SEARCH, 
  SELECTED
} m1_status=STANDBY;

long m1_prevTime;
long m1_waitTime = 5000;



// //////////////
// ANIMATOR STUFF
// //////////////

#include "Animator_Sine.h"
Animator_Sine m1_animSine;

#include "Animator_AR.h"
Animator_AR m1_animAR;



// /////////
// LED STUFF
// /////////

// this is the order of the colors GREEN, RED, BLUE
#include "Adafruit_NeoPixel.h"
#include <SPI.h> // see Adafruit DotStar library example to better understand

// the number of LEDs per single strip
#define NLEDS 21

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip( NLEDS, LEDSTRIP_DATA_PIN, NEO_GRBW + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
float m1_y = 0.0;

// TODO: change here something
int m1_intensity = 255; 
int m1_color = 0;
// these values are used during the "breathing" phase
int m1_max_intensity = 60; 
int m1_led_offset = 0;
// an utility variable to light up the LEDs
int m1_led_counter = m1_led_offset;
int m1_tailSize = 2;

// /////////////
// ENCODER STUFF 
// /////////////

// Encoder code is inspired by the work of eran.io
// http://eran.io/rotary-encoder-based-cooking-timer/
// Here's code: https://github.com/pavius/rotary-encoder-timer

#include "eran_encoder.h"
// main counter
long m1_counter = 0;


EranEncoder m1_encoder; 

void m1_cwStep(void)
{
  m1_counter --;
  
  m1_led_counter --;
  if( m1_led_counter < m1_led_offset) 
    m1_led_counter = (m1_led_offset+NLEDS)-1;
  
  m1_status = SEARCH;
  m1_prevTime = millis();
  //Serial.println("1,+1;");
  Serial.println("-1|0");
}

void m1_ccwStep(void)
{
  m1_counter ++;
  
  m1_led_counter ++;
  if( m1_led_counter >= m1_led_offset + NLEDS) m1_led_counter = m1_led_offset;
  
  m1_status = SEARCH;
  m1_prevTime = millis();
  //Serial.println("1,-1;");
  Serial.println("+1|0");
}

// ////////////
// BUTTON STUFF
// ////////////

#include "ButtonDebounce.h"

// (ButtonDebounce Library is written and maintained 
// by Maykon L. Capellari <maykonluiscapellari@gmail.com>
// https://github.com/maykon/ButtonDebounce

ButtonDebounce m1_push(M1_ENC_PUSH_PIN, 50);

void m1_pushChanged(int state){
  if( !state )
  {
    m1_status = SELECTED;
    m1_animAR.trigger();
    m1_prevTime = millis();
    //Serial.println("1,P;");
    Serial.println("0|1");
  }
}

// ///////
// UTILITY
// ///////
int i, j = 0;

// SETUP ///////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);

  // LED STUFF 
  strip.begin(); // Initialize pins for output
  // set every pixel to sleep
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, 0x00, 0x00, 0x00, 0x00);
  }
  strip.show();  // Turn all LEDs off ASAP 

  // MODULE 1

  pinMode(M1_ENC_CHA_PIN, INPUT_PULLUP);
  pinMode(M1_ENC_CHB_PIN, INPUT_PULLUP);
  
  m1_encoder.init(M1_ENC_CHA_PIN, M1_ENC_CHB_PIN);
  m1_encoder.setCallbackCW(m1_cwStep);
  m1_encoder.setCallbackCCW(m1_ccwStep);
  m1_push.setCallback(m1_pushChanged);
  m1_animSine.init(0.5, 0.0);
  m1_animAR.init(100, 500);
}

// LOOP ////////////////////////////////////////////////////////////////

void loop() 
{

  // MODULE 1
  
  m1_encoder.update();
  m1_push.update();

  if( m1_status == STANDBY )
  {
    m1_animSine.update();
    m1_y = m1_animSine.getY();
  } 
  else if( m1_status == SEARCH )
  {
    m1_y = 0;
    if( (millis()-m1_prevTime) < m1_waitTime )
    {
      // do nothing
    }
    else
    {
      m1_status = STANDBY;
      m1_prevTime = millis();
    }
  }
  else if( m1_status == SELECTED )
  {
    m1_animAR.update();
    m1_y = m1_animAR.getY();
    if( (millis()-m1_prevTime) < m1_waitTime )
    {
      // do nothing
    }
    else
    {
      m1_status = STANDBY;
      m1_prevTime = millis();
    }
  }

  // LED STUFF

  if( m1_status == STANDBY || m1_status == SELECTED )
  {
    for(i=0; i<NLEDS; i++)
    {
      m1_color = m1_y*m1_y*m1_max_intensity;
      strip.setPixelColor(i + m1_led_offset, 0,0,0,m1_color);
    }
  } 
  else
  {
    // we are in SEARCH state

    // first shutdown every LEDS
    for(i=m1_led_offset; i<m1_led_offset+NLEDS; i++) {
      strip.setPixelColor(i, 0,0,0,0);
    }

    // then light up only the LEDs which makes part of the tail
    // if the tail is 1 LED long so we will light up only one LED
    // if the tail is K LED long we will light up all the K LEDs (in a wrapped mode)

    lightTail( m1_led_counter );
  }
  
  strip.show();

  //m1PrintStates();
  
  delay(1); 
}


void lightTail( int firstLed ) {
  // light up all the LEDs composing the tail
  int tailIndex;
  for(int j=0; j<m1_tailSize; j++) {
    tailIndex = (firstLed-m1_led_offset) + j;
    tailIndex %= NLEDS;
    strip.setPixelColor(tailIndex+m1_led_offset, 0,0,0, m1_intensity);
  }
  
}

void m1PrintStates() 
{
  if( m1_status == STANDBY )   {
    Serial.println("STANDBY");
  }
  else if( m1_status == SEARCH )   {
    Serial.println("SEARCH");
  }
  else if ( m1_status == SELECTED )   {
    Serial.println("SELECTED");
  } 
  else  {
    Serial.println("??");
  }
}
