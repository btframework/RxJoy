/*
 * RX PWM Reader
 * 
 * Copyright (c) 2019 Mike Petrichenko
 * http://www.tlmviewer.com
 * https://www.btframework.com
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/
#include <EnableInterrupt.h>

#include "pwm.h"

// RX channels to PINs map.
const uint8_t CHANNEL_PINS[MAX_CHANNELS] = { 2, 3, 4, 5, 6, 7, 8, 9 };

// Indicates that the PWM value(s) has been updated by interrupt
// service routine.
volatile bool IsUpdated = false;
// Holds the current PWM values calculated in interrupt service rountine.
volatile uint16_t SharedPwm[MAX_CHANNELS];

uint16_t Pwm[MAX_CHANNELS];

// Common code for interrupts.
#define RxInterrupt(Channel) \
{ \
  static uint32_t tStart;\
  if (digitalRead(CHANNEL_PINS[Channel]) == HIGH) \
    tStart = micros(); \
  else \
  { \
    uint32_t tCurrent = micros(); \
    SharedPwm[Channel] = (uint16_t)(tStart > tCurrent ? 1 + tStart + ~tCurrent : tCurrent - tStart); \
    IsUpdated = true; \
  } \
}

// Interrupt service rountines.
void RxChannel0Interrupt() RxInterrupt(0)
void RxChannel1Interrupt() RxInterrupt(1)
void RxChannel2Interrupt() RxInterrupt(2)
void RxChannel3Interrupt() RxInterrupt(3)
void RxChannel4Interrupt() RxInterrupt(4)
void RxChannel5Interrupt() RxInterrupt(5)
void RxChannel6Interrupt() RxInterrupt(6)
void RxChannel7Interrupt() RxInterrupt(7)

// Setup PIN CHANGE interrupts
void AttachInterrupts()
{
  enableInterrupt(CHANNEL_PINS[0], RxChannel0Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[1], RxChannel1Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[2], RxChannel2Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[3], RxChannel3Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[4], RxChannel4Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[5], RxChannel5Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[6], RxChannel6Interrupt, CHANGE);
  enableInterrupt(CHANNEL_PINS[7], RxChannel7Interrupt, CHANGE);
}

// Copies PWM values from shared memory to the memory used by main LOOP.
bool GetChannels()
{
  // Check shared update flags to see if any channels have a new signal
  if (IsUpdated)
  {
    // Turn interrupts off while we take local copies of the shared variables.
    noInterrupts();
      memcpy(Pwm, (void*)SharedPwm, sizeof(uint16_t) * MAX_CHANNELS);
      IsUpdated = false;
    interrupts();
    
    // We have to check channels range. Acceptable range is 1000 - 2000
    for (uint8_t c = 0; c < MAX_CHANNELS; c++)
    {
      if (Pwm[c] < 1000)
        Pwm[c] = 1000;
      else if (Pwm[c] > 2000)
        Pwm[c] = 2000;
    }
    return true;
  }
  return false;
}
