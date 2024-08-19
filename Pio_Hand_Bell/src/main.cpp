/*
 Copyright 2024 Tauno Erik

 By Tauno Erik
 Modufied 17.08.2024

*/

#include <Arduino.h>
#include <Servo.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"


const int SERVO_PIN = 15;
Servo bell_servo;
#define DELAY_TIME 2
#define MAX_POS 180

uint slice_num = 0;

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

void setup() {
  // bell_servo.attach(SERVO_PIN);

  // Set the PWM pin
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    // Get the PWM slice number (a slice controls two pins, but we use one)
    slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    // Set the PWM frequency (50Hz for typical servo motors)
    pwm_set_clkdiv(slice_num, 64.f);  // Prescaler to get a suitable frequency
    pwm_set_wrap(slice_num, 39062);   // 50Hz (20ms period) with system clock

    // Enable PWM
    pwm_set_enabled(slice_num, true);
}

void loop() {
  int pos;
  /*
  // goes from 0 degrees to 180 degrees
  for (pos = 0; pos <= MAX_POS; pos += 1) {
    // in steps of 1 degree
    bell_servo.write(pos);
    delay(DELAY_TIME);
  }

  delay(1000);

  // goes from 180 degrees to 0 degrees
  for (pos = MAX_POS; pos >= 0; pos -= 1) {
    bell_servo.write(pos);
    delay(DELAY_TIME);
  }

  delay(1000);
  */
 /*
    bell_servo.writeMicroseconds(700);         
    delay(2000);                      
    bell_servo.writeMicroseconds(2300);        
    delay(2000);
    */

  servo_up(slice_num, 50);
  delay(1000);
  servo_down(slice_num, 50);
  delay(1000);
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}