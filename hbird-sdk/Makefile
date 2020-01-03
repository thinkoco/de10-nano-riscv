#############################################################
# Configuration
#############################################################

BIN2IHEX ?= $(abspath ../tools/bin2ihex)
# Pointers to various important tools in the toolchain.
RISCV_PATH ?=/opt/riscv-none-gcc/7.2.0-2-20180111-2230/
RISCV_OPENOCD ?= openocd

FLASHXIP    := flashxip
FLASH       := flash
ITCM        := itcm
IECM        := iecm
DOWNLOAD    := itcm

FTDI   := ftdi
HBIRD  := hbird
OLMX   := olmx
OCDCFG := ${FTDI}


E203        := e203
CORE        := e203

CORE_NAME = $(shell echo $(CORE) | tr a-z A-Z)
core_name = $(shell echo $(CORE) | tr A-Z a-z)

REPLACE_PRINTF  := 0
USE_NANO        := 1
NANO_PFLOAT     := 1

ifeq ($(core_name),${E203}) 
RISCV_ARCH := rv32imac
RISCV_ABI  := ilp32
endif

# Default target
PROGRAM ?= demo_gpio
GDB_PORT ?= 3333

#############################################################
# Prints help message
#############################################################
.PHONY: help
help:
	@echo "  HummingBird RISC-V Embedded Processor Software Development Kit "
	@echo "  Makefile targets:"
	@echo ""
	@echo " software [PROGRAM=$(PROGRAM)]:"
	@echo "    Build a software program to load with the"
	@echo "    debugger."
	@echo ""
	@echo " dasm [PROGRAM=$(PROGRAM)]:"
	@echo "     Generates the dissassembly output of 'objdump -D' to stdout."
	@echo ""
	@echo " upload [PROGRAM=$(PROGRAM)]:"
	@echo "    Launch OpenOCD to flash your program to the"
	@echo "    on-board Flash."
	@echo ""
	@echo " run_openocd :"
	@echo ""
	@echo " run_gdb     [PROGRAM=$(PROGRAM)]:"
	@echo "     Launch OpenOCD or GDB seperately. Allows Ctrl-C to halt running"
	@echo "     programs."
	@echo ""
	@echo " clean [PROGRAM=$(PROGRAM)]:"
	@echo "     clean generated files."
	@echo ""
	@echo " update the RISCV_PATH in Makefile first !!!!!!!!!!!!"

RISCV_GCC     := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-gcc)
RISCV_GXX     := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-g++)
RISCV_OBJDUMP := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-objdump)
RISCV_OBJCOPY := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-objcopy)
RISCV_GDB     := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-gdb)
RISCV_AR      := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-ar)
RISCV_SIZE    := $(abspath $(RISCV_PATH)/bin/riscv-none-embed-size)

PATH := $(abspath $(RISCV_PATH)/bin):$(PATH)



#############################################################
# This Section is for Software Compilation
#############################################################
PROGRAM_DIR = software/$(PROGRAM)
PROGRAM_ELF = software/$(PROGRAM)/$(PROGRAM)

.PHONY: clean
clean:
	$(MAKE) -C $(PROGRAM_DIR) BSP_BASE=$(abspath hbird_e203_bsp)  DOWNLOAD=$(DOWNLOAD) clean

.PHONY: software
software: clean
	$(MAKE) -C $(PROGRAM_DIR) SIZE=$(RISCV_SIZE) CC=$(RISCV_GCC) RISCV_ARCH=$(RISCV_ARCH) REPLACE_PRINTF=$(REPLACE_PRINTF)  NANO_PFLOAT=$(NANO_PFLOAT) USE_NANO=$(USE_NANO) DOWNLOAD=$(DOWNLOAD)  RISCV_ABI=$(RISCV_ABI) AR=$(RISCV_AR) OBJCOPY=$(RISCV_OBJCOPY) OBJDUMP=$(RISCV_OBJDUMP) BSP_BASE=$(abspath hbird_e203_bsp) 

dasm: software
	$(MAKE) dasm -C $(PROGRAM_DIR) BIN2IHEX=$(BIN2IHEX) OBJCOPY=$(RISCV_OBJCOPY) OBJDUMP=$(RISCV_OBJDUMP) BSP_BASE=$(abspath hbird_e203_bsp) 

#############################################################
# This Section is for uploading a program to SPI Flash
#############################################################
OPENOCD_UPLOAD = hbird_e203_bsp/tools/openocd_upload.sh
ifeq ($(OCDCFG),${OLMX})
OPENOCDCFG ?= hbird_e203_bsp/env/openocd_olmx.cfg
endif
ifeq ($(OCDCFG),${FTDI})
OPENOCDCFG ?= hbird_e203_bsp/env/openocd_ftdi.cfg
endif
ifeq ($(OCDCFG),${HBIRD})
OPENOCDCFG ?= hbird_e203_bsp/env/openocd_hbird.cfg
endif
OPENOCDARGS += -f $(OPENOCDCFG)

GDB_UPLOAD_ARGS ?= --batch

GDB_UPLOAD_CMDS += -ex "set remotetimeout 240"
GDB_UPLOAD_CMDS += -ex "target extended-remote localhost:$(GDB_PORT)"
GDB_UPLOAD_CMDS += -ex "monitor reset halt"
GDB_UPLOAD_CMDS += -ex "monitor flash protect 0 0 last off"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "monitor resume"
GDB_UPLOAD_CMDS += -ex "monitor shutdown"
GDB_UPLOAD_CMDS += -ex "quit"

openocd_upload:
	$(OPENOCD_UPLOAD) $(PROGRAM_ELF) $(OPENOCDCFG)

upload:
	$(RISCV_OPENOCD) $(OPENOCDARGS) & \
	$(RISCV_GDB) $(PROGRAM_DIR)/$(PROGRAM) $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS) && \
	echo "Successfully uploaded '$(PROGRAM)'."

#############################################################
# This Section is for launching the debugger
#############################################################

run_openocd:
	$(RISCV_OPENOCD) $(OPENOCDARGS)

GDBCMDS += -ex "set remotetimeout 240"
GDBCMDS += -ex "target extended-remote localhost:$(GDB_PORT)"

run_gdb:
	$(RISCV_GDB) $(PROGRAM_DIR)/$(PROGRAM) $(GDBARGS) $(GDBCMDS)
