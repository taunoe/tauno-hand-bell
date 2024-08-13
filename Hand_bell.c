/*
 *  Copyright 2024 Tauno Erik
 *  File:    Hand_bell.c
 *  Started: 11.08.2024
 *  Edited:  11.08.2024
 *  TODO:
 *  - mootor
 *  - web
 *  - sleep
 * 
 * https://cocode.se/linux/raspberry/pwm.html
 */
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"


#define SERVO_PIN 15

// Convert angle in degrees to duty cycle percentage
float angle_to_duty_cycle(float angle) {
    // 0.5ms pulse for 0 degrees, 2.5ms pulse for 180 degrees
    return 2.5f + (angle / 180.0f) * 10.0f;
}

void servo_up(uint slice_num, uint delay_val) {
    for (float angle = 0; angle <= 180; angle += 10) {
        float duty_cycle = angle_to_duty_cycle(angle);
        // Convert to PWM register value
        pwm_set_chan_level(slice_num,
                           pwm_gpio_to_channel(SERVO_PIN),
                           duty_cycle * 390.62);
        // printf("Up!\n");
        // Wait a bit for the servo to reach the position
        sleep_ms(delay_val);
    }
}

void servo_down(uint slice_num, uint delay_val) {
    for (float angle = 180; angle >= 0; angle -= 10) {
        float duty_cycle = angle_to_duty_cycle(angle);
        // Convert to PWM register value
        pwm_set_chan_level(slice_num,
                           pwm_gpio_to_channel(SERVO_PIN),
                           duty_cycle * 390.62);
        // printf("Down!\n");
        // Wait a bit for the servo to reach the position
        sleep_ms(delay_val);
    }
}

/* */
bool send_to_core_1(int32_t data) {
    // VER 1.
    // Send data to Core 1
    // multicore_fifo_push_blocking(data);

    // VER 2.
    printf("Core 0 sending: %d\n", data);
    // Non-blocking push
    bool success = multicore_fifo_push_timeout_us(data, 0);

    if (!success) {
        return false;
        printf("Core 0: FIFO is full. Could not send %d\n", data);
    }

    return true;
}


/**********************************************************************/
// Function to be run on Core 1
void core_1_entry() {
    while (true) {
        /*
        // VER 1.
        // Get data from Core 0
        int32_t data_from_core0 = multicore_fifo_pop_blocking();
        printf("Core 1 received: %d\n", data_from_core0);
        */

        // VER 2.
        int32_t data_from_core_0;
        // Non-blocking pop
        bool success = multicore_fifo_pop_timeout_us(0, &data_from_core_0);
        if (success) {
            printf("Core 1 received: %d\n", data_from_core0);
        } else {
            // printf("Core 1: No data received.\n");
        }

        printf("Tauno Hand Bell\n");
        sleep_ms(500);
    }
}

/**********************************************************************/
int main() {
    stdio_init_all();

    // Set the PWM pin
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // Get the PWM slice number (a slice controls two pins, but we use one)
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Set the PWM frequency (50Hz for typical servo motors)
    pwm_set_clkdiv(slice_num, 64.f);  // Prescaler to get a suitable frequency
    pwm_set_wrap(slice_num, 39062);   // 50Hz (20ms period) with system clock

    // Enable PWM
    pwm_set_enabled(slice_num, true);

    int counter = 0;

    // Launch core1_entry() on Core 1
    multicore_launch_core1(core_1_entry);

    while (true) {
        sleep_ms(5000);
        servo_up(slice_num, 50);
        sleep_ms(5000);
        servo_down(slice_num, 50);

        send_to_core_1(counter);

        sleep_ms(500);  // Sleep for 0.5 second

        counter++;
    }
}
