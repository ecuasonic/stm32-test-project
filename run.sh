#!/bin/bash

main() {
    cmake -B build -S . \
        -DTEST_NAME=stm
    cmake --build build

    st-flash --reset write ./build/firmware.bin 0x08000000
}

main