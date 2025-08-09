#include "types.h"
#include "util.h"
#include "systick.h"
#include "core_stm/gpio.h"
#include "core_stm/rcc.h"

// =================================
//              RCC
// =================================
#define CLEAR_MCO       ~RCC_CFGR_MCO(0x7)
#define SET_MCO(mco)    RCC_CFGR_MCO(mco)
// --
#define SET_MCO_HSI     SET_MCO(0x5)

// =================================
//              GPIO
// =================================
#define CLEAR_CNF(pin)          (~GPIO_CRH_CNF(pin, 0x3))
#define CLEAR_MODE(pin)         (~GPIO_CRH_MODE(pin, 0x3))
#define CLEAR_PIN(pin)          CLEAR_CNF(pin) & CLEAR_MODE(pin)
#define SET_CNF(pin, cnf)       GPIO_CRH_CNF(pin, cnf)
#define SET_MODE(pin, mode)     GPIO_CRH_MODE(pin, mode)
// --
#define SET_CNF_IN_PUPD(pin)    SET_CNF(pin, 0x2)
#define SET_CNF_OUT_PP(pin)     SET_CNF(pin, 0x0)
#define SET_CNF_OUT_AF_PP(pin)  SET_CNF(pin, 0x2)
// --
#define SET_MODE_IN(pin)        SET_MODE(pin, 0x0)
#define SET_MODE_OUT_2MHZ(pin)  SET_MODE(pin, 0x2)
#define SET_MODE_OUT_10MHZ(pin) SET_MODE(pin, 0x1)
#define SET_MODE_OUT_50MHZ(pin) SET_MODE(pin, 0x3)

static void config_rcc(void) {
    // MCO[2:0]
    RCC->CFGR &= CLEAR_MCO;
    RCC->CFGR |= SET_MCO_HSI;

    // APB2 Peripheral Clock Enable Register
    RCC->APB2ENR |= RCC_APB2ENR_IOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_IOAEN;
}

static void config_gpio(void) {
    // LED Output
    GPIO('C')->CRH &= CLEAR_PIN(13);
    GPIO('C')->CRH |= SET_MODE_OUT_2MHZ(13) | SET_CNF_OUT_PP(13);

    // MCO AF Output
    GPIO('A')->CRH &= CLEAR_PIN(8);
    GPIO('A')->CRH |= SET_MODE_OUT_50MHZ(8) | SET_CNF_OUT_AF_PP(8);
}

static void setup(void) {
    config_rcc();
    config_gpio();
}

int main(void) {
    setup();

    for (;;) {
        uint32_t timer, period = 500;
        if (timer_expired(&timer, period, s_ticks)) {
            static uint32_t set;
            if (set) {
                gpio_set('C', 13);
            } else {
                gpio_clear('C', 13);
            }
            set = ~set;
        }
    }
}
