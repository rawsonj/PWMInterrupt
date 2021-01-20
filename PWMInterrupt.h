/* PWMInterrupt - read internal temperature of ARM processor
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

/*
 * This implements a driver that will execute callbacks based on PWM
 * output.
 *
 * Originally developed to interface with an RC car receiver, it should be
 * general enough that people can find it useful.
 *
 * It supports two modes of operation:
 *   1. A 2-mode switch with an "off" below some theshold, and "on" above
 *      that threshold.
 *   2. A 3-mode switch with "off", "on1" and "on2."
 *
 * The actual implementation is to check if the pointer is void first,
 * then execute the callback if it is not void for all of the positions.
 *
 * The pulses look like:
 *                         _________________
 * _______________________/                 \_______________________
 *                        <---pulse width--->
 *
 * Since this is interrupt driven, the period doesn't matter.
 */

#ifndef PWMInterrupt_h_
#define PWMInterrupt_h_

typedef void (*voidFuncPtr)(void);

class PWMInterrupt {
    public:
        PWMInterrupt(int pin_num_arg,
                     int pw,
                     voidFuncPtr function,
                     voidFuncPtr function1,
                     bool invert_logic);

        PWMInterrupt(int pin_num_arg,
                     int pw1,
                     int pw2,
                     voidFuncPtr function,
                     voidFuncPtr function1,
                     voidFuncPtr function2,
                     bool invert_logic);
        void begin();

    private:
        voidFuncPtr callback1; // Callback for when the delta is below both durations.
        voidFuncPtr callback2; // Callback for when duration1 is reached.
        voidFuncPtr callback3; // Callback for when duration2 is reached.
        unsigned long duration1; // 1st pulse duration, in microseconds.
        unsigned long duration2; // 2nd pulse duration, in case you're using a 3 position switch on the pwm.
        int pin_num; // Stores the pin number this interrupt is attached to.
        void service(); // service callback that implements the main logic.
        unsigned long start; // Used to save the edge time.
        bool invert; // Uses the falling-edge as the start instead of the rising edge.
};

#endif
