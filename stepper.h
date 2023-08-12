#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <cstdint>

class StepperMotor
{
public:
    StepperMotor(){}
    
    StepperMotor(uint8_t pin1, uint8_t pin2, 
                uint8_t pin3, uint8_t pin4, 
                float sleep_time = 0.002);
    void single_step();
    void step(int n_step);

private:
    uint8_t pins[4];
    float sleep_time;
    const int (*seq)[4];
    int current_seq_step;

    void setup_pins();
};

#endif
