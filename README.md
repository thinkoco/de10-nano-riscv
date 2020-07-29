# DE10-NANO-RISCV

A RISC-V SoC on Terasic DE10-NANO

- [x] [DE10-Nano](https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=167&No=1046)
- [ ] [T-Core](https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=218&No=1230) (As far as I known, terasic has ported the E203 to T-Core,which is a sweet board)

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
- [x] add hbird-sdk runtime (testing)
- [x] add a simple C-based RISC-V simulator that can run led blink binary
- [x] update the ftdi script for e203 debug with ft2232hl tools (upload the binary to itcm, then run openocd and gdb)

## Build the itcm ram hex

### 02.led_c

    riscv-none-embed-gcc -march=rv32i -mabi=ilp32 -Tlink_itcm.lds  -nostartfiles -o led.elf start.S main.c
	riscv-none-embed-objcopy -O binary led.elf led.bin
	../../../tools/bin2ihex led.bin > led.hex

### hbird-sdk
```shell
    cd de10-nano-riscv/hbird-sdk
    export RISCV_OPENOCD= path to openocd (eg./opt/xpack-openocd-0.10.0-14/bin)
    export RISCV_PATH= path to risc-v gcc path (eg./opt/riscv-none-gcc/7.2.0-2-20180111-2230)
    make dasm PROGRAM=hello_world DOWNLOAD=itcm USE_NANO=1 NANO_PFLOAT=0
    make upload PROGRAM=hello_world DOWNLOAD=itcm
    make run_openocd PROGRAM=hello_world DOWNLOAD=itcm
open a new terminal
    cd de10-nano-riscv/hbird-sdk
    export RISCV_OPENOCD= path to openocd (eg./opt/xpack-openocd-0.10.0-14/bin)
    export RISCV_PATH= path to risc-v gcc path (eg./opt/riscv-none-gcc/7.2.0-2-20180111-2230)
    make run_gdb PROGRAM=hello_world DWONLOAD=itcm

(gdb)
    break main

    jump main
    
    l

    n
```
here is the [riscv-none-gcc link](https://github.com/ilg-archived/riscv-none-gcc/releases/download/v7.2.0-2-20180110/gnu-mcu-eclipse-riscv-none-gcc-7.2.0-2-20180111-2230-centos64.tgz) 
## run simulation with verilator
   [brabect1's e200_opensource repo](https://github.com/brabect1/e200_opensource)

1. [install verilator and gtkwave](https://github.com/brabect1/e200_opensource/blob/master/verilator/README.md)
2. make build test-rv32ui-p-xori cflags='-DVCDTRACE=1'
3. gtkwave dump.vcd