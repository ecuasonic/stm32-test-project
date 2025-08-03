#include "types.h"

struct uart {
  volatile uint32_t SR;   // Status Register
  volatile uint32_t DR;   // Data Register
  volatile uint32_t BRR;  // Baud Rate Register
  volatile uint32_t CR1;  // Control Register 1
  volatile uint32_t CR2;  // Control Register 2
  volatile uint32_t CR3;  // Control Register 3
  volatile uint32_t GTPR; // Guard Time and Prescalar Register
};
