#include "stepper.h"
#include "pico/stdlib.h"
#include <cstdlib>
#include <stdio.h>

const int full_phase_stepping_seq[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1}
};

StepperMotor::StepperMotor(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4, float sleep_time)
    : sleep_time(sleep_time), seq(full_phase_stepping_seq), current_seq_step(0)
{
    pins[0] = pin1;
    pins[1] = pin2;
    pins[2] = pin3;
    pins[3] = pin4;
    setup_pins();
}

void StepperMotor::setup_pins()
{
    for (int pin = 0; pin < 4; pin++)
    {
        gpio_init(pins[pin]);
        gpio_set_dir(pins[pin], GPIO_OUT);
        gpio_put(pins[pin], false);
    }
}

void StepperMotor::single_step()
{
    printf("Stepping seq %d \n", current_seq_step );
    printf("[");
    for (int pin = 0; pin < 4; pin++)
    {
        gpio_put(pins[pin], seq[current_seq_step][pin]);
        printf("%d, ", seq[current_seq_step][pin]);
    }
    printf("]\n");
    sleep_ms(static_cast<int>(sleep_time * 1000));
}

void StepperMotor::step(int n_step)
{
    int increment = (n_step >= 0) ? 1 : -1;

    for (int step = 0; step < abs(n_step); step++)
    {
        single_step();
        current_seq_step += increment;
        current_seq_step = (4 + (current_seq_step % 4)) % 4;
    }
}
