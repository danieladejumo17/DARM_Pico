// I2C Communication interface & control stepper motors on a RaspberryPi Pico device

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include <stdio.h>
#include "stepper.h"

#include <map>
#include <vector>
#include "stepper.h"

#define I2C_ADDR 0x46  // 0x3E, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4A, 0x4C


class RPiPicoDevice {
public:
    RPiPicoDevice(int cmd_nsteps, std::map<int, std::vector<uint8_t>> motor_pins, 
                    std::vector<int> motor_gains
    ) : cmd_nsteps(cmd_nsteps), motor_pins(motor_pins), motor_gains(motor_gains) {
        create_stepper_motors();
    }

    void act(std::vector<int> cmd) {
        motor0.step(cmd[0] * cmd_nsteps * motor_gains[0]);
        motor1.step(cmd[1] * cmd_nsteps * motor_gains[1]);
        motor2.step(cmd[2] * cmd_nsteps * motor_gains[2]);
        motor3.step(cmd[3] * cmd_nsteps * motor_gains[3]);
        motor4.step(cmd[4] * cmd_nsteps * motor_gains[4]);
        motor5.step(cmd[5] * cmd_nsteps * motor_gains[5]);
    }

private:
    int cmd_nsteps;
    std::map<int, std::vector<uint8_t>> motor_pins;
    std::vector<int> motor_gains;
    StepperMotor motor0;
    StepperMotor motor1;
    StepperMotor motor2;
    StepperMotor motor3;
    StepperMotor motor4;
    StepperMotor motor5;

    void create_stepper_motors() {
        motor0 = StepperMotor(motor_pins[0][0], motor_pins[0][1], motor_pins[0][2], motor_pins[0][3]);
        motor1 = StepperMotor(motor_pins[1][0], motor_pins[1][1], motor_pins[1][2], motor_pins[1][3]);
        motor2 = StepperMotor(motor_pins[2][0], motor_pins[2][1], motor_pins[2][2], motor_pins[2][3]);
        motor3 = StepperMotor(motor_pins[3][0], motor_pins[3][1], motor_pins[3][2], motor_pins[3][3]);
        motor4 = StepperMotor(motor_pins[4][0], motor_pins[4][1], motor_pins[4][2], motor_pins[4][3]);
        motor5 = StepperMotor(motor_pins[5][0], motor_pins[5][1], motor_pins[5][2], motor_pins[5][3]);
    }
};

void bytes_to_trinary(const uint8_t byte_array[2], uint8_t trinary_array[6]) {
    // Combine the two bytes into a decimal value
    int decimal_value = byte_array[0] * 256 + byte_array[1];

    // Convert decimal value to trinary digits
    for (int i = 5; i >= 0; i--) {
        trinary_array[i] = decimal_value % 3;
        decimal_value /= 3;
    }
}

void byteToTrinary(uint8_t byte, uint8_t trinaryDigits[5]) {
    for (int i = 4; i >= 0; i--) {
        trinaryDigits[i] = byte % 3;
        byte /= 3;
    }
}

int main() {
    stdio_init_all();

    // i2c setup
    i2c_init(i2c1, 10000);
    i2c_set_slave_mode(i2c1, true, I2C_ADDR);
    gpio_set_function(2, GPIO_FUNC_I2C);
    gpio_set_function(3, GPIO_FUNC_I2C);
    gpio_pull_up(2);
    gpio_pull_up(3);

    // Pico Device
    // Define the motor pins
    std::map<int, std::vector<uint8_t>> motor_pins = {
        {0, {4,5,6,7}},
        {1, {8,9,10,11}},
        {2, {12,13,14,15}},
        {3, {16,17,18,19}},
        {4, {20,21,22,26}},
        {5, {27,28,0,1}
    }};

    // Define the motor gains
    std::vector<int> motor_gains = {5, 5, 5, 5, 5, 5};

    // Create an instance of RPiPicoDevice
    RPiPicoDevice pico_device(1, motor_pins, motor_gains);

    uint8_t rxdata[2];
    while (true) {
        // Receive data from controller
        // 3 bytes received - byte 0 is cmd (used as lower byte) byte 2 is higher - byte 3 is 0
        if (i2c_get_read_available(i2c1) < 2) 
            continue;

        i2c_read_raw_blocking (i2c1, rxdata, 2);
        printf("Received [%d, %d]\n", rxdata[0], rxdata[1]);
        
        uint8_t trinary_digits[6];
        bytes_to_trinary(rxdata, trinary_digits);
        
        printf("Converted trinary digits: ");
        for (int i = 0; i < 6; i++) {
            printf("%u ", trinary_digits[i]);
        }
        printf("\n");


        // Actuate the motors
        // Shift from [0, 2] to [-1, 1]
        int trinary_digits_int[6];
        for (int i = 0; i < 6; i++) {
            trinary_digits_int[i] = trinary_digits[i] - 1;
            // trinary_digits_int[i] = trinary_digits_int[i] - 1;
        }
        printf("Shifted trinary digits: ");
        for (int i = 0; i < 6; i++) {
            printf("%d ", trinary_digits_int[i]);
        }
        printf("\n");
        std::vector<int> cmd(trinary_digits_int, trinary_digits_int + sizeof(trinary_digits_int) / sizeof(int));
        pico_device.act(cmd);
    }
}