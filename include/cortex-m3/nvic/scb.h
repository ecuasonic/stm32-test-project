#ifndef SCB_H
#define SCB_H

#include "types.h"

struct SCB {
    // ==========================================
    // CPUID Base (CPUID) (r)
    //    Reset value:
    //        ???
    //    Bits:
#define SCB_CPUID_IMPLEMENTER   (0x0FF << 24)
#define SCB_CPUID_VARIANT       (0x00F << 20)
#define SCB_CPUID_Constant      (0x00F << 16)
#define SCB_CPUID_PARTNO        (0xFFF << 4)
#define SCB_CPUID_REVISION      (0x00F << 0)
    uint32_t CPUID;

    // ==========================================
    // Interrupt Control State Register (ICSR)
    //    Reset value:
    //        0x0000_0000
    //    Bits:
#define SCB_ICSR_NMIPENDSET     (1 << 31)
#define SCB_ICSR_PENDSVSET      (1 << 28)
#define SCB_ICSR_PENDSVCLR      (1 << 27)
#define SCB_ICSR_PENDSTSET      (1 << 26)
#define SCB_ICSR_PENDSTCLR      (1 << 25)
#define SCB_ICSR_ISRPREEMPT     (1 << 23)
#define SCB_ICSR_ISPENDING      (1 << 22)
#define SCB_ICSR_VECTPENDING(n) (((n)&3FF) << 12)
#define SCB_ICSR_RETTOBASE      (1 << 11)
#define SCB_ICSR_VECTACTIVE(n)  (((n)&0x1FF) << 0)
    uint32_t ICSR;

    // ==========================================
    // Vector Table Offset Register (VTOR)
    //    Reset value:
    //        ???
    //    Bits:
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