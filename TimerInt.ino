// Phil Martel noodling around with timer interrupts
// This code is based on Amanda Ghassaei's2012 Instructible
// https://www.instructables.com/Arduino-Timer-Interrupts/

// Here's another relevant Instructable https://www.instructables.com/Coding-Timers-and-Delays-in-Arduino/
#include <Streaming.h> // This lets Serial use the C++ '<<' operator

// clock speed in Hz for Uno and Nano
#define CLOCK_MHZ 16

void setup() {
  // put your setup code here, to run once:
  pinMode( LED_BUILTIN, OUTPUT );
  
  Timer0Period( 1000.);
  
  interrupts();
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
#ifdef DEBUG
  float  f, f1;
  
  Serial.setTimeout( 60000L );
  f=1.;
  Serial << endl;
  Serial << "uSec? ";
  f = Serial.parseFloat();
  f1 = Timer0Period( f );
  interrupts();
#endif
}

ISR(TIMER0_COMPA_vect){//timer0 interrupt 2kHz toggles pin 8
  static int count = 0;

  if ( ++count >= 1000 ) {
  count = 0;
  digitalWrite( LED_BUILTIN, !digitalRead( LED_BUILTIN ) );
  }
}


float Timer0Period( float uSec ) {
  float retVal, p;
  int i, reg, pre[] ={1, 8, 64, 256, 1024};
  bool ok = false;
  
  // the timer interrupt period depends on the prescaler, which can have values of
  // 1, 8, 64, 256 or 1024 and the Output Counter Compare Register, OCCR0A, 
  // which can have values of 1 to 255.  For accuracy, you want to use the smallest 
  // prescaler value you can.  OCCRA = uSec / (16 * prescaler)
  p = 255. / CLOCK_MHZ;
  for ( i = 0; i < 5; i++ ) {
    if ( p * pre[i] >= uSec ) {
      ok = true;
      break;
    }
  }
  if (!ok) {
    Serial << uSec << " usec is too slow. Limit is " << p * pre[i-1] << endl;
    return -1.;
  }
  //Serial << " uSec = " << uSec << " i " << i << " pre = " << pre[i] << endl;
  reg = (int)(CLOCK_MHZ * uSec / pre[i] + 0.5);
  retVal = (float)reg * pre[i]/CLOCK_MHZ;
  //Serial << "reg= " << reg << " retVal= " << retVal << endl;
  
  //set timer0 interrupt rate
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B

  OCR0A = reg;  // set compare match register

  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set  prescaler
  TCCR0B |= (i+1)& 7;

  TCNT0 = 0;//initialize counter value to 0
  TIMSK0 |= (1 << OCIE0A);  // enable timer compare interrupt

  return retVal;
}
