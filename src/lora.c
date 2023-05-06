#include "lora.h"
#include "spi_master.h"

uint8_t lora_register_read(uint8_t address) {
    spi_master_buffer_t message = {{address & 0x7F, 0x00}, 2};
    while (spi_master_transfer(&message) == PT_BUSY) {
    };
    return message.data[2];
}

uint8_t lora_register_write(uint8_t address, uint8_t value) {
    spi_master_buffer_t message = {{address | 0x80, value}, 2};
    while (spi_master_transfer(&message) == PT_BUSY) {
    };
    return message.data[2];
}

bool lora_init(void) {
    spi_master_init();
    if (lora_register_read(LORA_REG_VERSION) != 0x12) {
        return false;
    }
    lora_sleep();

    return true;
}

bool lora_set_frequency(uint32_t frequency_hz) {
    uint32_t lora_reg_frf = ((uint64_t)frequency_hz << 19UL) / 32000000UL;
    lora_register_write(LORA_REG_FRF_MSB, (uint8_t)(lora_reg_frf >> 16));
    lora_register_write(LORA_REG_FRF_MID, (uint8_t)(lora_reg_frf >> 8));
    lora_register_write(LORA_REG_FRF_LSB, (uint8_t)(lora_reg_frf >> 0));
}

bool lora_set_spreading_factor(uint8_t sf) {
    if (sf < 6) {
        sf = 6;
    } else if (sf > 12) {
        sf = 12;
    }

    if (sf == 6) {
        lora_register_write(LORA_REG_DETECTION_OPTIMIZE, 0xC5);
        lora_register_write(LORA_REG_DETECTION_THRESHOLD, 0x0C);
    } else {
        lora_register_write(LORA_REG_DETECTION_OPTIMIZE, 0xC3);
        lora_register_write(LORA_REG_DETECTION_THRESHOLD, 0x0A);
    }

    uint8_t lora_reg_modem_config_2 = lora_register_read(LORA_REG_MODEM_CONFIG_2);
    lora_register_write(LORA_REG_MODEM_CONFIG_2, (lora_reg_modem_config_2 & 0x0F) | ((sf << 4) & 0xF0));

    setLdoFlag();
}

void lora_mode_idle(void) { lora_register_write(LORA_REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY); }

void lora_mode_sleep(void) { lora_register_write(LORA_REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP); }

void lora_mode_receive(void) { lora_register_write(LORA_REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_RX_CONTINUOUS); }

void lora_setLdoFlag() {
    // Section 4.1.1.5
    long symbolDuration = 1000 / (getSignalBandwidth() / (1L << getSpreadingFactor()));

    // Section 4.1.1.6
    boolean ldoOn = symbolDuration > 16;

    uint8_t config3 = readRegister(REG_MODEM_CONFIG_3);
    bitWrite(config3, 3, ldoOn);
    writeRegister(REG_MODEM_CONFIG_3, config3);
}