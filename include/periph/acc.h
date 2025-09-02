#ifndef ACC_H
#define ACC_H

#include "types.h"
#define ACC_I2C_ADDR 0x0C

extern uint32_t acc_configured;

uint32_t config_acc(void);
uint32_t get_single_data_acc(char port, uint32_t pin, int32_t *dest);
uint32_t get_test_acc(char port, uint32_t pin, int32_t *dest);
void format_data_acc(int32_t *raw, int32_t *offset, int32_t *formatted);

#endif
