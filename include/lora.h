#ifndef LORA_H
#define LORA_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// Lora HopeRF95W Registers
#define LORA_REG_FIFO                 0x00
#define LORA_REG_OP_MODE              0x01
#define LORA_REG_FRF_MSB              0x06
#define LORA_REG_FRF_MID              0x07
#define LORA_REG_FRF_LSB              0x08
#define LORA_REG_PA_CONFIG            0x09
#define LORA_REG_OCP                  0x0b
#define LORA_REG_LNA                  0x0c
#define LORA_REG_FIFO_ADDR_PTR        0x0d
#define LORA_REG_FIFO_TX_BASE_ADDR    0x0e
#define LORA_REG_FIFO_RX_BASE_ADDR    0x0f
#define LORA_REG_FIFO_RX_CURRENT_ADDR 0x10
#define LORA_REG_IRQ_FLAGS            0x12
#define LORA_REG_RX_NB_BYTES          0x13
#define LORA_REG_PKT_SNR_VALUE        0x19
#define LORA_REG_PKT_RSSI_VALUE       0x1a
#define LORA_REG_RSSI_VALUE           0x1b
#define LORA_REG_MODEM_CONFIG_1       0x1d
#define LORA_REG_MODEM_CONFIG_2       0x1e
#define LORA_REG_PREAMBLE_MSB         0x20
#define LORA_REG_PREAMBLE_LSB         0x21
#define LORA_REG_PAYLOAD_LENGTH       0x22
#define LORA_REG_MODEM_CONFIG_3       0x26
#define LORA_REG_FREQ_ERROR_MSB       0x28
#define LORA_REG_FREQ_ERROR_MID       0x29
#define LORA_REG_FREQ_ERROR_LSB       0x2a
#define LORA_REG_RSSI_WIDEBAND        0x2c
#define LORA_REG_DETECTION_OPTIMIZE   0x31
#define LORA_REG_INVERTIQ             0x33
#define LORA_REG_DETECTION_THRESHOLD  0x37
#define LORA_REG_SYNC_WORD            0x39
#define LORA_REG_INVERTIQ2            0x3b
#define LORA_REG_DIO_MAPPING_1        0x40
#define LORA_REG_VERSION              0x42
#define LORA_REG_PA_DAC               0x4d

// modes
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03
#define MODE_RX_CONTINUOUS       0x05
#define MODE_RX_SINGLE           0x06

// PA config
#define PA_BOOST                 0x80

// IRQ masks
#define IRQ_TX_DONE_MASK           0x08
#define IRQ_PAYLOAD_CRC_ERROR_MASK 0x20
#define IRQ_RX_DONE_MASK           0x40

#define RF_MID_BAND_THRESHOLD    525E6
#define RSSI_OFFSET_HF_PORT      157
#define RSSI_OFFSET_LF_PORT      164

#define MAX_PKT_LENGTH           255

bool lora_init(void);
uint8_t lora_read_register(uint8_t address);


#ifdef __cplusplus
} // extern "C"
#endif

#endif /* LORA_H */