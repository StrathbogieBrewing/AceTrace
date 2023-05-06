#include <avr/interrupt.h>
#include <avr/io.h>

#include "spi_master.h"

static volatile uint8_t spi_master_xfer_bytes_remaining = 0;
static volatile uint8_t *spi_master_xfer_data_pointer = 0;

ISR(SPI_STC_vect) {
    uint8_t *data_pointer = (uint8_t *)spi_master_xfer_data_pointer;
    *data_pointer++ = SPDR;
    if (spi_master_xfer_bytes_remaining != 0) {
        spi_master_xfer_bytes_remaining--;
        SPDR = *data_pointer;
        spi_master_xfer_data_pointer = data_pointer;
    } else {
        PORTB |= (1 << PORTB2);
    }
}

// ISR(SPI_STC_vect) {
//     // uint8_t *data_pointer = (uint8_t *)spi_master_xfer_data_pointer;
//     *spi_master_xfer_data_pointer++ = SPDR;
//     if (spi_master_xfer_bytes_remaining != 0) {
//         spi_master_xfer_bytes_remaining--;
//         SPDR = *spi_master_xfer_data_pointer;

//     } else {
//         PORTB |= (1 << PORTB2);
//     }
// }

// ISR(SPI_STC_vect) {
//     uint8_t index = spi_master_buffer.size;
//     if (index) {
//         index--;
//         spi_master_buffer.size = index;
//         uint8_t rx_data = SPDR;
//         SPDR = spi_master_buffer.data[index];
//         spi_master_buffer.data[index] = rx_data;
//     } else {
//         spi_master_buffer.data[index] = SPDR;
//         // SPCR = 0;
//         PORTB |= (1 << PORTB2);
//     }
// }

void spi_master_init(void) {
    DDRB |= (1 << PORTB2) | (1 << PORTB3) | (1 << PORTB5);
    PORTB |= (0 << PORTB3) | (1 << PORTB2) | (0 << PORTB5);
    SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (0 << CPHA) | (0 << CPOL) | (1 << SPIE);
}

// bool spi_master_write(spi_master_buffer_t *buffer) {
//     cli();
//     if ((PORTB & (1 << PORTB2)) == 0) {
//         sei();
//         return SPI_MASTER_BUSY;
//     }
//     spi_master_xfer_size = buffer->size;
//     uint8_t *data_ptr = buffer->data + buffer->size;
//     spi_master_buffer.size = 0;
//     while (data_ptr != buffer->data) {
//         data_ptr--;
//         if (data_ptr != buffer->data) {
//             spi_master_buffer.data[spi_master_buffer.size] = *data_ptr;
//             spi_master_buffer.size++;
//         } else {
//             // SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (0 << CPHA) | (0 << CPOL) | (1 << SPIE);
//             // SPCR = (1 << MSTR) | (1 << SPE) | (1 << SPR0) | (1 << SPR1) | (1 << CPHA) | (1 << CPOL) | (1 << SPIE);
//             PORTB &= ~(1 << PORTB2);
//             SPDR = *data_ptr;
//         }
//     }
//     sei();
//     return SPI_MASTER_DONE;
// }

// bool spi_master_read(spi_master_buffer_t *buffer) {
//     cli();
//     if ((PORTB & (1 << PORTB2)) == 0) {
//         sei();
//         return SPI_MASTER_BUSY;
//     }
//     uint8_t *data_ptr = buffer->data + buffer->size;
//     while (spi_master_xfer_size) {
//         spi_master_xfer_size--;
//         *data_ptr = spi_master_buffer.data[spi_master_xfer_size];
//         data_ptr--;
//     }
//     sei();
//     return SPI_MASTER_DONE;
// }

// typedef struct spi_master_xfer_ctx_t {
//     PT_CONTROL;
//     // spi_master_buffer_t buffer;
// } spi_master_xfer_ctx_t;

// static spi_master_xfer_ctx_t spi_master_xfer_ctx = {0};

// PT_THREAD spi_master_transfer(spi_master_buffer_t *buffer) {

//     PT_BEGIN(&spi_master_xfer_ctx);
//     if ((buffer->size > SPI_MASTER_BUFFER_SIZE) || (buffer->size == 0)) {
//         buffer->size = 0;
//         PT_EXIT(&spi_master_xfer_ctx);
//     }
//     PT_WAIT_WHILE(&spi_master_xfer_ctx, spi_master_write(buffer) == SPI_MASTER_BUSY);
//     PT_WAIT_WHILE(&spi_master_xfer_ctx, spi_master_read(buffer) == SPI_MASTER_BUSY);
//     PT_END(&spi_master_xfer_ctx);
// }

PT_THREAD spi_master_transfer(spi_master_buffer_t *buffer) {
    static struct {
        PT_CONTROL;
        // uint8_t size;
    } ctx = {0};

    PT_BEGIN(&ctx);

    if ((buffer->size > SPI_MASTER_BUFFER_SIZE) || (buffer->size == 0)) {
        buffer->size = 0;
        PT_EXIT(&ctx);
    }

    PT_WAIT_WHILE(&ctx, ((PORTB & (1 << PORTB2)) == 0));

    cli();
    PORTB &= ~(1 << PORTB2);
    spi_master_xfer_data_pointer = buffer->data;
    spi_master_xfer_bytes_remaining = buffer->size - 1;
    SPDR = *spi_master_xfer_data_pointer;
    sei();

    PT_WAIT_WHILE(&ctx, ((PORTB & (1 << PORTB2)) == 0));

    PT_END(&ctx);
}

// uint8_t index = 0;
// while ((spi_master_read_tail != spi_master_tail) && (index < size)) {
//     data[index] = spi_master_buffer.data[spi_master_read_tail];
//     index += 1;
//     spi_master_read_tail += 1;
//     spi_master_read_tail &= SPI_MASTER_BUFFER_MASK;
// }
// sei();
// return index;

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