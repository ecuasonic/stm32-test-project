#ifndef SCB_H
#define SCB_H

#include "types.h"

struct SCB {
    // CPUID Base
    uint32_t CPUID;

    // Interrupt Control and State
    uint32_t ICSR;

    // Vector Table Offset
    uint32_t VTOR;

    // Application Interrupt & Reset Control
    uint32_t AIRCR;

    // System Control
    uint32_t SCR;

    // Configuration and Control
    uint32_t CCR;

    // System Handler Priority (3x4 bytes)
    uint32_t SHPR[12];

    // System Handler Control and State
    uint32_t SHCSR;

    // Configurable Fault Status
    uint32_t CFSR;

    // HardFault Status
    uint32_t HFSR;

    // Debug Fault Status
    uint32_t DFSR;

    // MemManage Fault Address
    uint32_t MMFAR;

    // BusFault Address
    uint32_t BFAR;

    // Auxiliary Fault Status
    uint32_t AFSR;

    // Processor Feature
    uint32_t PFR[2];

    // Debug Feature
    uint32_t DFR;

    // Auxiliary Feature
    uint32_t ADR;

    // Memory Model Feature
    uint32_t MMFR[4];

    // Instruction Set Attribute
    uint32_t ISAR[5];

    uint32_t RESERVED0;

    // Coprocessor Access Control (if FPU present)
    uint32_t CPACR;
};

#define SCB ((struct SCB *)0xE000ED00UL)

#endif // SCB_H