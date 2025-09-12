#ifndef ACC_H
#define ACC_H

#include "types.h"

// Refer to the AsahiKASEI AK8975 datasheet
#define ACC_I2C_ADDR 0x0C

struct acc {
    uint32_t addr;
    uint32_t configured;
    struct i2c *i2c;
};

uint32_t config_acc(struct acc *acc, struct i2c *i2c, uint32_t addr);
uint32_t get_test_acc(struct acc *acc, char port, uint32_t pin, int32_t *dest);
uint32_t get_single_data_acc(struct acc *acc, char port, uint32_t pin, int32_t *dest);
void format_data_acc(int32_t *raw, int32_t *offset, int32_t *formatted);

#endif
