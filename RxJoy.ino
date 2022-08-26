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
#include <PPMReader.h>
#include "usb.h"

int interruptPin = 2;
int channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

RxJoystick Joy;

void setup()
{
  // Sends a clean report to the host. This is important on any Arduino type.
  Joy.begin();
}

void loop()
{
  Joy.Throttle(ppm.latestValidChannelValue(3, 0));
  Joy.Aileron(ppm.latestValidChannelValue(1, 0));
  Joy.Elevator(ppm.latestValidChannelValue(2, 0));
  Joy.Rudder(ppm.latestValidChannelValue(4, 0));
  Joy.Gear(ppm.latestValidChannelValue(5, 0));
  Joy.Aux1(ppm.latestValidChannelValue(6, 0));
  Joy.write();
}
