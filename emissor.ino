/*
 * SendRawDemo.cpp - demonstrates sending IR codes with sendRaw
 *
 * This example shows how to send a RAW signal using the IRremote library.
 * The example signal is actually a 32 bit NEC signal.
 * Remote Control button: LGTV Power On/Off.
 * Hex Value: 0x20DF10EF, 32 bits
 *
 * If it is a supported protocol, it is more efficient to use the protocol send function
 * (here sendNEC) to send the signal.
 *
 *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
 *
 ************************************************************************************
 * MIT License
 *
 * Copyright (c) 2020-2022 Armin Joachimsmeyer
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ************************************************************************************
 */
#include <Arduino.h>

//#define SEND_PWM_BY_TIMER         // Disable carrier PWM generation in software and use (restricted) hardware PWM.
//#define USE_NO_SEND_PWM           // Use no carrier PWM, just simulate an active low receiver signal. Overrides SEND_PWM_BY_TIMER definition
//#define NO_LED_FEEDBACK_CODE      // Saves 566 bytes program memory
//#define USE_OPEN_DRAIN_OUTPUT_FOR_SEND_PIN // Use or simulate open drain output mode at send pin. Attention, active state of open drain is LOW, so connect the send LED between positive supply and send pin!

#include "PinDefinitionsAndMore.h" // Define macros for input and output pin etc.
#include <IRremote.hpp>

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
#if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/|| defined(SERIALUSB_PID) || defined(ARDUINO_attiny3217)
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
#endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

#if defined(IR_SEND_PIN)
    IrSender.begin(); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin
#else
    IrSender.begin(3, ENABLE_LED_FEEDBACK); // Specify send pin and enable feedback LED at default feedback LED pin
#endif

    Serial.println(F("Ready to send IR signals at pin "  STR(IR_SEND_PIN)));
}

/*
 * NEC address=0xFB0C, command=0x18
 *
 * This is data in byte format.
 * The uint8_t/byte elements contain the number of ticks in 50 us.
 * The uint16_t format contains the (number of ticks * 50) if generated by IRremote,
 * so the uint16_t format has exact the same resolution but requires double space.
 * With the uint16_t format, you are able to modify the timings to meet the standards,
 * e.g. use 560 (instead of 11 * 50) for NEC or use 432 for Panasonic. But in this cases,
 * you better use the timing generation functions e.g. sendNEC() directly.
 */
const uint8_t rawDataP[] PROGMEM
= { 180, 90 /*Start bit*/, 11, 11, 11, 11, 11, 34, 11, 34/*0011 0xC of 16 bit address LSB first*/, 11, 11, 11, 11, 11, 11, 11,
        11/*0000*/, 11, 34, 11, 34, 11, 11, 11, 34/*1101 0xB*/, 11, 34, 11, 34, 11, 34, 11, 34/*1111*/, 11, 11, 11, 11, 11, 11, 11,
        34/*0001 0x08 of command LSB first*/, 11, 34, 11, 11, 11, 11, 11, 11/*1000 0x01*/, 11, 34, 11, 34, 11, 34, 11,
        11/*1110 Inverted 8 of command*/, 11, 11, 11, 34, 11, 34, 11, 34/*0111 inverted 1 of command*/, 11 /*stop bit*/};

void loop() {

//#if !(defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__))
    /*
     * Send hand crafted data from RAM
     * The values are NOT multiple of 50, but are taken from the NEC timing definitions
     */
    Serial.println(F("Send NEC 16 bit address=0xFB04 and command 0x08 with exact timing (16 bit array format)"));
    Serial.flush();

    const uint16_t rawData[] = {8991, 4515, 623, 1686, 622, 553, 654, 557, 649, 584, 622, 1686, 622, 1685, 623, 583, 623, 583, 623, 1684, 624, 1684, 623, 1686, 623, 581, 625, 583, 624, 559, 647, 584, 622, 582, 624, 583, 624, 555, 651, 584, 622, 559, 648, 584, 622, 1658, 651, 583, 624, 583, 623, 584, 623, 583, 622, 583, 624, 556, 649, 1658, 651, 584, 622, 1685, 624, 585, 622, 559, 647, 1686, 621, 583, 624, 20019, 624, 583, 623, 558, 649, 583, 622, 582, 625, 584, 622, 584, 622, 586, 621, 582, 624, 585, 623, 582, 623, 583, 624, 585, 622, 583, 623, 582, 649, 1662, 622, 1685, 622, 583, 624, 554, 653, 583, 623, 582, 625, 583, 623, 583, 624, 558, 648, 583, 623, 583, 624, 584, 623, 584, 622, 584, 622, 584, 623, 1686, 622, 1686, 623, 1684, 623, 40030, 8967, 4514, 621, 584, 623, 583, 624, 583, 622, 584, 624, 584, 648, 557, 623, 585, 622, 583, 623, 585, 622, 582, 650, 556, 625, 556, 675, 558, 624, 582, 624, 583, 649, 557, 624, 557, 701, 532, 648, 559, 672, 534, 622, 584, 647, 560, 622, 584, 623, 584, 648, 533, 647, 585, 622, 555, 676, 560, 621, 585, 647, 1660, 623, 582, 624, 1660, 671, 560, 622, 1685, 649, 558, 647, 19968, 674, 557, 649, 559, 621, 584, 623, 556, 650, 584, 648, 558, 650, 558, 623, 583, 623, 558, 673, 558, 648, 559, 647, 560, 648, 558, 647, 558, 649, 558, 649, 557, 648, 559, 623, 585, 646, 559, 648, 558, 623, 583, 649, 559, 648, 558, 623, 583, 647, 559, 624, 583, 649, 558, 648, 558, 649, 558, 649, 1659, 622, 585, 647, 1635, 648}; // Using exact NEC timing
    IrSender.sendRaw(rawData, sizeof(rawData) / sizeof(rawData[0]), NEC_KHZ); // Note the approach used to automatically calculate the size of the array.

    delay(3000); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal
//#endif

    /*
     * Send byte data direct from FLASH
     * Note the approach used to automatically calculate the size of the array.
     
    Serial.println(F("Send NEC 16 bit address 0xFB0C and data 0x18 with (50 us) tick resolution timing (8 bit array format) "));
    Serial.flush();
    IrSender.sendRaw_P(rawDataP, sizeof(rawDataP) / sizeof(rawDataP[0]), NEC_KHZ);

    delay(1000); // delay must be greater than 5 ms (RECORD_GAP_MICROS), otherwise the receiver sees it as one long signal

    Serial.println(F("Send NEC 16 bit address 0x0102, 8 bit data 0x34 with generated timing"));
    Serial.flush();
    IrSender.sendNEC(0x0102, 0x34, true, 0);

    delay(1000);
    */
}
