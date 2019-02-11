/*
 * USB HID Joystick
 * 
 * Copyright (c) 2019 Mike Petrichenko
 * http://www.tlmviewer.com
 * https://www.btframework.com
 * 
 * The code based on NicoHood HID library.
 * https://github.com/NicoHood/HID
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

#include "common.h"
#include "usb.h"

RxJoystick Joy;

// Serial port between IO MCU and USB MCU
#define hwSerial  Serial1
//#define usbSerial Serial

void setup()
{
  // Sends a clean report to the host. This is important on any Arduino type.
  Joy.begin();

  // Start USB Serial
  //usbSerial.begin(0);

  // Init hardware serial port.
  hwSerial.end();
  hwSerial.begin(57600);
}

uint16_t read()
{
  while (hwSerial.available() < 2) ;
  return (hwSerial.read() << 8) | hwSerial.read();
}

void loop()
{
  // Wait for sync byte.
  uint16_t sync = read;
  while (sync != SYNC)
  {
    while (!hwSerial.available()) ;
    sync = (sync << 8) | hwSerial.read();
  }

  uint16_t channels[MAX_CHANNELS];
  for (uint8_t c = 0; c < MAX_CHANNELS; c++)
    channels[c] = read();
  
  /*usbSerial.print("CH0: "); usbSerial.print(channels[0]); usbSerial.print(" ");
  usbSerial.print("CH1: "); usbSerial.print(channels[1]); usbSerial.print(" ");
  usbSerial.print("CH2: "); usbSerial.print(channels[2]); usbSerial.print(" ");
  usbSerial.print("CH3: "); usbSerial.print(channels[3]); usbSerial.print(" ");
  usbSerial.print("CH4: "); usbSerial.print(channels[4]); usbSerial.print(" ");
  usbSerial.print("CH5: "); usbSerial.print(channels[5]); usbSerial.print(" ");
  usbSerial.print("CH6: "); usbSerial.print(channels[6]); usbSerial.print(" ");
  usbSerial.print("CH7: "); usbSerial.println(channels[7]);*/
  
  Joy.Throttle(channels[0]);
  Joy.Aileron(channels[1]);
  Joy.Elevator(channels[2]);
  Joy.Rudder(channels[3]);
  Joy.Gear(channels[4]);
  Joy.Aux1(channels[5]);
  Joy.Aux2(channels[6]);
  Joy.Aux3(channels[7]);
  
  Joy.write();
}
