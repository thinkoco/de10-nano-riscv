set_global_assignment -entity "riscv_pll" -library "lib_riscv_pll" -name IP_TOOL_NAME "altera_pll"
set_global_assignment -entity "riscv_pll" -library "lib_riscv_pll" -name IP_TOOL_VERSION "18.1"
set_global_assignment -entity "riscv_pll" -library "lib_riscv_pll" -name IP_TOOL_ENV "mwpim"
set_global_assignment -library "lib_riscv_pll" -name SPD_FILE [file join $::quartus(sip_path) "riscv_pll.spd"]

set_global_assignment -library "lib_riscv_pll" -name MISC_FILE [file join $::quartus(sip_path) "riscv_pll_sim/riscv_pll.vo"]
