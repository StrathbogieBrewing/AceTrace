#include <avr/io.h>
#include <util/delay.h>

#include "lora.h"

int main(void) {
    lora_init();
    while (1) {
        _delay_ms(10);
        lora_read_register(LORA_REG_VERSION);
    }
    return 0;
}