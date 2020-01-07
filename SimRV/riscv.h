
typedef unsigned int REG32;

#define ITCM_ADDR 0x80000000
#define ITCM_SIZE 0x40000
#define LED_OUT_CTRL    0x10012008
#define LED_OUT_VALU    0x1001200C
#define UART_VALU       0x40000000

// state
enum STATE {INIT,FETCH,DECODE,EXECUTE, \
            EXE_WR_TYPE_2REG,EXE_WR_TYPE_2MEM, \
            EXE_WR_TYPE_MEM2REG,EXE_WR_REG_FROM_MEM,EXE_WR_TYPE_NOP} st;

#ifdef DEBUG
#include <stdarg.h>
static int debug_info(const char *format, ...)
{
    va_list argPtr;
    int     count;
    va_start(argPtr, format);                  /*  获取可变参数列表  */
    fflush(stdout);                            /*  强制刷新输出缓冲区  */
    count = vfprintf(stderr, format, argPtr);  /*  将信息输出到标准出错流设备  */
    va_end(argPtr);                            /*  可变参数列表结束  */
}
#else
static inline int debug_info(const char *format, ...)
{
}
#endif

#define _OPCODE5(a)  get_bits(a,6,2)
#define _OPCODE7(a)  get_bits(a,6,0)
#define _RS1_IDX(a)  get_bits(a,19,15)
#define _RS2_IDX(a)  get_bits(a,24,20)
#define _RD_IDX(a)   get_bits(a,11,7)
#define _FUNC3(a)    get_bits(a,11,7)

//                         func7_func3_opcode  
#define      LUI       "15'bxxxxxxx_xxx_01101"
#define      AUIPC     "15'bxxxxxxx_xxx_00101"
#define      JAL       "15'bxxxxxxx_xxx_11011"
#define      JALR      "15'bxxxxxxx_000_11001"
#define      BEQ       "15'bxxxxxxx_000_11000"
#define      BNE       "15'bxxxxxxx_001_11000"
#define      BLT       "15'bxxxxxxx_100_11000"
#define      BGE       "15'bxxxxxxx_101_11000"
#define      BLTU      "15'bxxxxxxx_110_11000"
#define      BGEU      "15'bxxxxxxx_111_11000"

#define      LB        "15'bxxxxxxx_000_00000"
#define      LH        "15'bxxxxxxx_001_00000"
#define      LW        "15'bxxxxxxx_010_00000"
#define      LBU       "15'bxxxxxxx_100_00000"
#define      LHU       "15'bxxxxxxx_101_00000"
#define      LWU       "15'bxxxxxxx_110_00000"
#define      LD        "15'bxxxxxxx_011_00000"

#define      SB        "15'bxxxxxxx_000_01000"
#define      SH        "15'bxxxxxxx_001_01000"
#define      SW        "15'bxxxxxxx_010_01000"
#define      SD        "15'bxxxxxxx_011_01000"

#define      ADDI      "15'bxxxxxxx_000_00100"
#define      ADDIW     "15'bxxxxxxx_000_00110"
#define      ADD       "15'b0000000_000_01100"
#define      ADDW      "15'b0000000_000_01110"
#define      SUB       "15'b0100000_000_01100"
#define      SUBW      "15'b0100000_000_01110"
#define      XORI      "15'bxxxxxxx_100_00100"
#define      XOR       "15'b0000000_100_01100"
#define      ORI       "15'bxxxxxxx_110_00100"
#define      OR        "15'b0000000_110_01100"
#define      ANDI      "15'bxxxxxxx_111_00100"
#define      AND       "15'b0000000_111_01100"
#define      SLLI      "15'b0000000_001_00100"
//#define      SLLI      "15'b000000?_001_00100"
#define      SLLIW     "15'b0000001_001_00110"
#define      SLL       "15'b0000000_001_01100"
#define      SLLW      "15'b0000000_001_01110"
#define      SLTI      "15'bxxxxxxx_010_00100"
#define      SLT       "15'b0000000_010_01100"
#define      SLTU      "15'b0000000_011_01100"
#define      SLTIU     "15'bxxxxxxx_011_00100"
#define      SRLI      "15'b0000000_101_00100"
//#define      SRLI      "15'b000000?_101_00100"
#define      SRLIW     "15'b0000000_101_00110"
#define      SRL       "15'b0000000_101_01100"
#define      SRLW      "15'b0000000_101_01110"
#define      SRAI      "15'b0100000_101_00100"
//#define      SRAI      "15'b010000?_101_00100"
#define      SRAIW     "15'b0100000_101_00110"
#define      SRA       "15'b0100000_101_01100"
#define      SRAW      "15'b0100000_101_01110"