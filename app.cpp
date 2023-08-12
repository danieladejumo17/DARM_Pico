#include <stdio.h>
#include "stepper.h"

int main() {
    StepperMotor motor(4,5,6,7);  // Replace the pin numbers with the actual pin numbers you are using

    printf("Testing StepperMotor class...\n");

    motor.step(2048);  // Step 100 times

    printf("Test complete.\n");

    return 0;
}
