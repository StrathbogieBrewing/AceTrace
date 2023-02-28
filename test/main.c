#include <avr/io.h>
#include <util/delay.h>

#include "spi_master.h"

int main(void) {
    spi_master_init();
    uint8_t counter = 0;
    while (1) {
        _delay_ms(10);
        counter++;
        spi_master_buffer_t message = {{55, counter, counter + 2}, 3};
        spi_master_write(&message);
    }
    return 0;
}