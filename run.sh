#!/bin/bash
TEST_NAME=""

msg() {
    echo "================="
    echo "run.sh TEST_NAME  "
    echo "                  "
    echo "TEST_NAME options:"
    echo "    i (interrupts)"
    echo "                  "
}

main() {
    case $1 in
        1|2)
            TEST_NAME="stm$1"
            ;;
        *)
            msg
            exit 0
            ;;
    esac

    cmake -B build -S . \
        -DTEST_NAME=${TEST_NAME}
    cmake --build build

    st-flash --reset write ./build/firmware.bin 0x08000000
}

main $1