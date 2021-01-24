PWMInterrupt
============

This is a library that implements a way to take as input a PWM signal
into an arduino-compatible microcontroller and do stuff with it.

The original inspiration for this was to interface with an RC car receiver
that would only output PWM signals of different widths to indicate
button presses or toggles.

You only need to know the widths of the pulses you are looking for.
Frequency of the signal is not required.

Everything is implemented in a call-back fashion and driven by interrupts
to make it as efficient as possible.

Below is an example of how to use it in a sketch

```cpp

#include "WProgram.h"
#include <PWMInterrupt.h>

#define OUTPUTPIN 13
#define PWMINPUT 2

void turn_off_output(void) {
    digitalWriteFast(OUTPUTPIN, LOW);
}

void turn_on_output(void) {
    digitalWriteFast(OUTPUTPIN, HIGH);
}

PWMInterrupt pwm_input(PWMINPUT, 1400, turn_off_output, turn_on_output, false);

void service(void) {
    pwm_input.service();
}

extern "C" int main(void)
{
    Serial.begin(115200);
    pinMode(OUTPUTPIN, OUTPUT);
    pinMode(PWMINPUT, INPUT);
    attachInterrupt(digitalPinToInterrupt(PWMINPUT), service, CHANGE); // Attach interrupt
    while (1) {
        // Do other, more important things here.
        delay(500);
        ;
    }
}
```

You should be able to instantiate several of these in the same sketch and define service methods
to call each instances service functions, then attach them to interrupts in your main().
