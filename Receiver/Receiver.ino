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
#include "pwm.h"

void setup()
{
  pinMode(13, OUTPUT);
  Serial.begin(57600);
  AttachInterrupts();
}

void write(uint16_t w)
{
  while (Serial.availableForWrite() < 2) ;
  Serial.write(w >> 8);
  Serial.write(w & 0x00FF);
}

void loop()
{
  digitalWrite(13, LOW);
  delay(25);
  if (GetChannels())
  {
    write(SYNC);
    for (uint8_t c = 0; c < MAX_CHANNELS; c++)
      write(Pwm[c]);
  }
  digitalWrite(13, HIGH);
  delay(25);
}

