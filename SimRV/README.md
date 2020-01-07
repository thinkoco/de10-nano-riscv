# A simple C-based RISC-V simulator

The C code shows that how the riscv32i core works in an easy way. Also, it can run led blink binary

## description

1. ported form [mcupro](https://github.com/mcupro/SimRV32I)
2. 64k itcm to load code
3. jum to 0x80000000 to run itcm code
4. gpio peripheral (output mode)
5. UART TX

## build the simulaitor

- [x] `gcc -o SimRV riscv.c -O3` (without debug informations)
- [x] `gcc -o SimRV riscv.c -O3 -DDBUG` (with debug informations)

## run the binary

	./SimRV led.bin
