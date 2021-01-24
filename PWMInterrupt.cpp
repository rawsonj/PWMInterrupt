/* PWMInterrupt
 * Copyright (C) 2021 Joe Rawson
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>

#include "PWMInterrupt.h"
#include <Arduino.h>

PWMInterrupt::PWMInterrupt(int pin_num_arg,
                           int pw1,
                           int pw2,
                           voidFuncPtr function,
                           voidFuncPtr function1,
                           voidFuncPtr function2,
                           bool invert_logic=false) {
    callback1 = function;
    callback2 = function1;
    duration1 = pw1;
    callback3 = function2;
    duration2 = pw2;
    pin_num = pin_num_arg;
    invert = invert_logic;
    pinMode(pin_num, INPUT);
}

PWMInterrupt::PWMInterrupt(int pin_num_arg,
                           int pw,
                           voidFuncPtr function,
                           voidFuncPtr function1,
                           bool invert_logic=false) {
    callback1 = function; // should be set to something, but we should also never hit this one.
    duration1 = pw;
    callback2 = function1;
    duration2 = pw;
    callback3 = function1;
    pin_num = pin_num_arg;
    invert = invert_logic;
}

// The service callback. Senses the pin level and calls the appropriate callback
// depending on the delay from the pulse width.
void PWMInterrupt::service() {
    unsigned long time = micros();
    uint8_t line_val = digitalReadFast(pin_num);
    unsigned long delta = 0;
    uint8_t check_val = (invert ? LOW: HIGH);
    // If the line is the start of the pulse, record the start time and exit.
    // Else, it's the end of the pulse, get the time, calculate the delta and
    // compare to the pulse duration values, then execute the callback, if defined.
    if (line_val == check_val) {
        start = time;
    } else {
        delta = time - start; // This will also be erroneous if we rolled over.
                              // Should think about the math harder here.
        if (duration2 <= delta) {
            callback3();
        } else if (duration1 <= delta) {
            callback2();
        } else {
            callback1();
        }

    }
}
