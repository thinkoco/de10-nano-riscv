# DE10-NANO-RISCV

A RISC-V SoC on Terasic DE10-NANO

## Description

1. Hbird E203 form [Nuclei Technology](https://www.nucleisys.com)
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

## No new plan

