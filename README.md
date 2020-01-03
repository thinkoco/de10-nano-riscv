# DE10-NANO-RISCV

A RISC-V SoC on Terasic DE10-NANO

## Description

1. Hbird E203 form [Nuclei Technology](https://github.com/SI-RISCV/e200_opensource)
2. Ported the code form [LiChee-Tang](https://github.com/Lichee-Pi/Tang_E203_Mini)
3. 16Mhz clock input now(can works higher)
4. 6927 LAMs and about 65Kb block ram now (maybe less)
5. mask-rom boot default, and jump to itcm ram 0x80000000
6. LED0 and LED2 blink default

## Works

- [x] change the pll and ram ip for intel FPGA
- [x] Write the LED-Blink ASM Code for E203
- [x] Build the binary and make a mif(hex) file for itcm ram
- [x] some timing check by signaltap and verdi simulation
- [x] write the binary to itcam hex tools
- [x] add led C code

## run simulation with verilator
   [brabect1's e200_opensource repo](https://github.com/brabect1/e200_opensource)

1. [install verilator and gtkwave](https://github.com/brabect1/e200_opensource/blob/master/verilator/README.md)
2. make build test-rv32ui-p-xori cflags='-DVCDTRACE=1'
3. gtkwave dump.vcd