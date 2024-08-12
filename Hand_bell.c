/*
 *  Copyright 2024 Tauno Erik
 *  File:    Hand_bell.c
 *  Started: 11.08.2024
 *  Edited:  11.08.2024
 *  TODO:
 *  - mootor
 *  - web
 *  - sleep
 */
#include <stdio.h>
#include "pico/stdlib.h"


int main() {
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
