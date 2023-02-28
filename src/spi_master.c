#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi_master.h"

static volatile spi_master_buffer_t spi_master_buffer = {0};
static uint8_t spi_master_transfer_size = 0;

ISR(SPI_STC_vect) {
    uint8_t index = spi_master_buffer.size;
    if (index) {
        index--;
        spi_master_buffer.size = index;
        uint8_t rx_data = SPDR;
        SPDR = spi_master_buffer.data[index];
        spi_master_buffer.data[index] = rx_data;
    } else {
        spi_master_buffer.data[index] = SPDR;
        SPCR = 0;
        PORTB |= (1 << PORTB2);
    }
}

void spi_master_init(void) {
    DDRB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5);
    PORTB |= (1 << PORTB3) | (1 << PORTB2) | (1 << PORTB5);
}

bool spi_master_write(spi_master_buffer_t *buffer) {
    if ((buffer->size > SPI_MASTER_BUFFER_SIZE) || (buffer->size == 0)) {
        return false;
    }
    spi_master_transfer_size = buffer->size;
    cli();
    if ((PORTB & (1 << PORTB2)) == 0) {
        sei();
        return false;
    }
    uint8_t *data_ptr = buffer->data + buffer->size;
    spi_master_buffer.size = 0;
    while (data_ptr != buffer->data) {
        data_ptr--;
        if (data_ptr != buffer->data) {
            spi_master_buffer.data[spi_master_buffer.size] = *data_ptr;
            spi_master_buffer.size++;
        } else {
            SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
            PORTB &= ~(1 << PORTB2);
            SPDR = *data_ptr;
        }
    }
    sei();
    return true;
}

bool spi_master_read(spi_master_buffer_t *buffer) {
    cli();
    if ((PORTB & (1 << PORTB2)) == 0) {
        sei();
        return false;
    }
    uint8_t *data_ptr = buffer->data + buffer->size;
    while (spi_master_transfer_size) {
        spi_master_transfer_size--;
        *data_ptr = spi_master_buffer.data[spi_master_transfer_size];
        data_ptr--;
    }

    sei();
    return true;

    // uint8_t index = 0;
    // while ((spi_master_read_tail != spi_master_tail) && (index < size)) {
    //     data[index] = spi_master_buffer.data[spi_master_read_tail];
    //     index += 1;
    //     spi_master_read_tail += 1;
    //     spi_master_read_tail &= SPI_MASTER_BUFFER_MASK;
    // }
    // sei();
    // return index;
}

//     SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
//     SPDR = data[index];
//     index += 1;
// }
// while (index < size) {
//     spi_master_buffer.data[spi_master_head] = data[index];
//     spi_master_head += 1;
//     spi_master_head &= SPI_MASTER_BUFFER_MASK;
//     data[index] = spi_master_buffer.data[spi_master_head];
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
//         spi_master_buffer.data[spi_master_head] = data[index];
//         spi_master_head += 1;
//         spi_master_head &= SPI_MASTER_BUFFER_MASK;
//         data[index] = spi_master_buffer.data[spi_master_head];
//         index += 1;
//     }
// }
// sei();
// }