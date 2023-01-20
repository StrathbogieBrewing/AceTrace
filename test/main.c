#include <avr/io.h>
#include <util/delay.h>

#include "trace.h"

int main(void) {
    trace_init();
    uint8_t counter = 0;
    while (1) {
        _delay_ms(10);
        counter++;
        trace_write((uint8_t[]){0x0F, counter, 0xF0}, 3);
    }
    return 0;
}