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

#pragma once

#include <Arduino.h>
#include <HID.h>

// For multiple reports support.
#define HID_REPORTID_RX_JOYSTICK  11

// USB Joystick HID device descriptor.
static const uint8_t HID_DESCRIPTOR_JOYSTICK[] PROGMEM =
{
  /* Joystick with 8 axis */
  0x05, 0x01,                     /* USAGE_PAGE (Generic Desktop) */
  0x09, 0x04,                     /* USAGE (Joystick) */
  0xa1, 0x01,                     /* COLLECTION (Application) */
  0x85, HID_REPORTID_RX_JOYSTICK, /*   REPORT_ID */
  /* 6 16bit Axis */
  0x05, 0x01,                     /*   USAGE_PAGE (Generic Desktop) */
  0x09, 0x01,                     /*   USAGE (Pointer) */
  0xa1, 0x00,                     /*   COLLECTION (Physical) */
  0x09, 0x36,                     /*     USAGE (Slider) for Throttle */
  0x09, 0x30,                     /*     USAGE (X) for Roll (Aileron) */
  0x09, 0x31,                     /*     USAGE (Y) for Pitch (Elevator)  */
  0x09, 0x32,                     /*     USAGE (Z) for Rudder */
  0x09, 0x33,                     /*     USAGE (Rx) for Gear */
  0x09, 0x34,                     /*     USAGE (Ry) for Aux 1 */
  0x16, 0xE8, 0x03,               /*     LOGICAL_MINIMUM (1000) */
  0x26, 0xD0, 0x07,               /*     LOGICAL_MAXIMUM (2000) */
  0x75, 0x10,                     /*     REPORT_SIZE (16) */
  0x95, 0x06,                     /*     REPORT_COUNT (6) */
  0x81, 0x02,                     /*     INPUT (Data,Var,Abs) */
  0xc0,                           /*   END_COLLECTION */
  0xc0                            /* END_COLLECTION */
};

// Internal data storage type.
typedef struct
{
  int16_t Throttle;
  int16_t Aileron;
  int16_t Elevator;
  int16_t Rudder;
  int16_t Gear;
  int16_t Aux1;
} HID_RX_JOYSTICK_REPORT;
  
class RxJoystick
{
private:
  HID_RX_JOYSTICK_REPORT FReport;

  inline void SendReport(void* data, int length)
  {
    HID().SendReport(HID_REPORTID_RX_JOYSTICK, data, length);
  }
  
public:
  RxJoystick()
  {
    static HIDSubDescriptor node(HID_DESCRIPTOR_JOYSTICK, sizeof(HID_DESCRIPTOR_JOYSTICK));
    HID().AppendDescriptor(&node);
  }
  
  inline void begin()
  {
    end();
  }
  
  void end()
  {
    memset(&FReport, 0x00, sizeof(FReport));
    SendReport(&FReport, sizeof(FReport));
  }
  
  inline void write()
  {
    SendReport(&FReport, sizeof(FReport)); 
  }
  
  inline void reset()
  {
    memset(&FReport, 0x00, sizeof(FReport)); 
  }

  inline void Throttle(int16_t a)
  {
    FReport.Throttle = a;
  }
  
  inline void Aileron(int16_t a)
  {
    FReport.Aileron = a; 
  }
  
  inline void Elevator(int16_t a)
  {
    FReport.Elevator = a; 
  }
  
  inline void Rudder(int16_t a)
  {
    FReport.Rudder = a; 
  }
  
  inline void Gear(int16_t a)
  {
    FReport.Gear = a; 
  }
  
  inline void Aux1(int16_t a)
  {
    FReport.Aux1 = a; 
  }
};
