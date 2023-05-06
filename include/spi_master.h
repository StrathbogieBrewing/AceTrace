#ifndef SPI_MASTER_H
#define SPI_MASTER_H

#include <stdbool.h>
#include <stdint.h>

#include "pt.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_MASTER_BUFFER_SIZE 16

#define SPI_MASTER_BUSY false
#define SPI_MASTER_DONE true

typedef struct spi_master_buffer_t {
    uint8_t data[SPI_MASTER_BUFFER_SIZE];
    uint8_t size;
} spi_master_buffer_t;

void spi_master_init(void);
// bool spi_master_write(spi_master_buffer_t *buffer);
// bool spi_master_read(spi_master_buffer_t *buffer);

PT_THREAD spi_master_transfer(spi_master_buffer_t *buffer);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* SPI_MASTER_H */