#include <avr/interrupt.h>
#include <avr/io.h>

#include <stdbool.h>

#include "trace.h"

#define TRACE_BUFFER_BITS (4)
#define TRACE_BUFFER_SIZE (1 << TRACE_BUFFER_BITS)
#define TRACE_BUFFER_MASK (TRACE_BUFFER_SIZE - 1)

static volatile uint8_t trace_data[TRACE_BUFFER_SIZE] __attribute__((aligned(TRACE_BUFFER_SIZE)));
static volatile uint8_t trace_index = 0;
static uint8_t trace_size = 0;

ISR(SPI_STC_vect) {
    uint8_t index = trace_index;
    if (index) {
        index--;
        trace_index = index;
        uint8_t rx_data = SPDR;
        SPDR = trace_data[index];
        trace_data[index] = rx_data;
    } else {
        trace_data[index] = SPDR;
        SPCR = 0;
        PORTB |= (1 << PORTB2);
    }
}

void trace_init(void) {
    DDRB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5);
    PORTB |= (1 << PORTB3) | (1 << PORTB2) | (1 << PORTB5);
}

bool trace_write(uint8_t data[], uint8_t size) {
    if ((size > TRACE_BUFFER_SIZE) || (size == 0)) {
        return false;
    }
    trace_size = size;
    cli();
    if ((PORTB & (1 << PORTB2)) == 0) {
        sei();
        return false;
    }
    uint8_t *data_ptr = data + size;
    trace_index = 0;
    while (data_ptr != data) {
        data_ptr--;
        if (data_ptr != data) {
            trace_data[trace_index] = *data_ptr;
            trace_index++;
        } else {
            SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
            PORTB &= ~(1 << PORTB2);
            SPDR = *data_ptr;
        }
    }
    sei();
    return true;
}



uint8_t trace_read(uint8_t data[], uint8_t max_size) {
    cli();
    if ((PORTB & (1 << PORTB2)) == 0) {
        sei();
        return 0;
    }
    sei();
    return true;



    // uint8_t index = 0;
    // while ((trace_read_tail != trace_tail) && (index < size)) {
    //     data[index] = trace_data[trace_read_tail];
    //     index += 1;
    //     trace_read_tail += 1;
    //     trace_read_tail &= TRACE_BUFFER_MASK;
    // }
    // sei();
    // return index;
}

//     SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
//     SPDR = data[index];
//     index += 1;
// }
// while (index < size) {
//     trace_data[trace_head] = data[index];
//     trace_head += 1;
//     trace_head &= TRACE_BUFFER_MASK;
//     data[index] = trace_data[trace_head];
//     index += 1;
// }
// }

// uint8_t index = 0;
// if (size) {
//     if ((PORTB & (1 << PORTB2)) != 0) {
//         PORTB &= ~(1 << PORTB2);
//         SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
//         SPDR = data[index];
//         index += 1;
//     }
//     while (index < size) {
//         trace_data[trace_head] = data[index];
//         trace_head += 1;
//         trace_head &= TRACE_BUFFER_MASK;
//         data[index] = trace_data[trace_head];
//         index += 1;
//     }
// }
// sei();
// }