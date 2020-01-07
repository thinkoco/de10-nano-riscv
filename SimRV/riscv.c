#include <stdio.h>
#include <stdlib.h>
#include "riscv.h"
#include "memory.c" 
#include "tool.c"

REG32 PC ;
REG32 INS; //instruction
REG32 RS1_IDX,RS2_IDX,RD_IDX; //reg source1 index ,reg source2 index , reg destination index  in instruction
REG32 RS1_VAL,RS2_VAL,RD_VAL; //reg source1 value ,reg source2 value , reg destination value in instruction
REG32 OP_CODE; // operation code  in instruction
REG32 FUNC3  ; // function 3 value in instruction
REG32 IMM_I  ;
REG32 IMM_S  ;
REG32 IMM_SB ;
REG32 IMM_U  ;
REG32 IMM_UJ ; 
REG32 RF[32] = {0} ; // register 0~31 in ALU
REG32 ALU_RES ;

void IF(){
	INS = read_memory_32bit(PC); 
    debug_info("IF %08x: %08x \n",PC,INS);
}

void ID (){
    RS1_IDX  =  get_bits(INS,19,15);
    RS2_IDX  =  get_bits(INS,24,20);
    RD_IDX   =  get_bits(INS,11,7);
    FUNC3   =   get_bits(INS,14,12);

    IMM_I   =  get_bits(INS,31,20);
    IMM_S   =  get_bits(INS,31,25)<<5 | get_bits(INS,11,7);
    IMM_SB  =  get_bits(INS,31,31)<<12 | get_bits(INS,30,25) <<5 |  get_bits(INS,7,7)  <<11 | get_bits(INS,11,8) << 1   ;
    IMM_U   =  get_bits(INS,31,12);
    IMM_UJ  =  get_bits(INS,31,31)<<20  |  get_bits(INS,30,21)<<1 |  get_bits(INS,20,20)<<11 | get_bits(INS,19,12)<<12 ; 
}

void RFe(){
	RS1_VAL =(RS1_IDX==0)?0: RF [ RS1_IDX ];
	RS2_VAL =(RS2_IDX==0)?0: RF [ RS2_IDX ];
}

void WR_REG_FROM_ALU(){  	RF[RD_IDX] = ALU_RES ;   }
void WR_REG_FROM_MEM(){  	RF[RD_IDX] = RD_VAL ;    }

void WR_MEM(){
    switch(FUNC3){
        case 0 : write_memory_8bit(RS2_VAL,ALU_RES);break;
        case 1 : write_memory_16bit(RS2_VAL,ALU_RES);break;
        case 2 : write_memory_32bit(RS2_VAL,ALU_RES);break;
        default:{printf("WR_MEM : Error value !\n");}
    }
}

void LD_MEM(){
    REG32 t;
    switch (FUNC3) {
        case 0: t = read_memory_8bit  ( ALU_RES ) ;RD_VAL = ext_s(t,7); break;
        case 1: t = read_memory_16bit  ( ALU_RES ) ;RD_VAL = ext_s(t,15); break;
        
        case 4:RD_VAL = read_memory_8bit  ( ALU_RES ) ;  break;
        case 5:RD_VAL = read_memory_16bit ( ALU_RES ) ; break;

        case 2:
        case 6:RD_VAL = read_memory_32bit ( ALU_RES ) ; break;
        default : printf("LD_MEM : Error Load type !\n");
        }     
}

void to_break_dead_loop(){
    static unsigned int old_PC;
    static unsigned int cntr = 0 ;
    if (old_PC == PC )  cntr ++ ; else {old_PC = PC ;cntr=0;}
    if (cntr ==10)  exit(1);
}

int main (int argc, char **argv)
{

    st = INIT;
    PC = ITCM_ADDR; // jump to itcm address
    printf("load binary file %s \n",argv[1]);
    load_prog(argv[1]);
    for(;;){
        to_break_dead_loop();
        switch(st){
            case INIT   : st = FETCH; break;
            case FETCH  : IF(); st = DECODE; break;
            case DECODE : ID();RFe(); st = EXECUTE; break;
            case EXECUTE:
                    if (cmp_ins (INS, LUI )) { debug_info("LUI ");    ALU_RES = IMM_U;ALU_RES<<=12; PC=PC+4; st= EXE_WR_TYPE_2REG ; }
                    else if (cmp_ins (INS, AUIPC )) { debug_info("AUIPC ");   ALU_RES =PC+  (IMM_U << 12);  PC=PC+4; st=EXE_WR_TYPE_2REG;  }
                    else if (cmp_ins (INS, JAL   )) { debug_info("JAL ");   ALU_RES = PC + 4 ;     PC=PC+ext_s( IMM_UJ,20);  st = EXE_WR_TYPE_2REG ;}
                    else if (cmp_ins (INS, JALR  )) { debug_info("JALR ");  ALU_RES = PC + 4 ;     PC =RS1_VAL + ext_s(IMM_I,11) ; st = EXE_WR_TYPE_2REG ; }

                    else if (cmp_ins (INS, BEQ   )) { debug_info("BEQ ");    if(RS1_VAL == RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }
                    else if (cmp_ins (INS, BNE   )) { debug_info("BNE ");    if(RS1_VAL != RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }
                    else if (cmp_ins (INS, BLT   )) { debug_info("BLT ");    if((int)RS1_VAL <  (int)RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }
                    else if (cmp_ins (INS, BGE   )) { debug_info("BGE ");    if((int)RS1_VAL >= (int)RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }
                    else if (cmp_ins (INS, BLTU  )) { debug_info("BLTU ");   if(RS1_VAL <  RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }
                    else if (cmp_ins (INS, BGEU  )) { debug_info("BGEU ");   if(RS1_VAL >= RS2_VAL) PC=PC+ext_s(IMM_SB,12);else PC=PC+4; st=EXE_WR_TYPE_NOP; }

                    else if (cmp_ins (INS, LB    )) { debug_info("LB ");    ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}
                    else if (cmp_ins (INS, LH    )) { debug_info("LH ");    ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}
                    else if (cmp_ins (INS, LW    )) { debug_info("LW ");    ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}
                    else if (cmp_ins (INS, LBU   )) { debug_info("LBU ");   ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}
                    else if (cmp_ins (INS, LHU   )) { debug_info("LHU ");   ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}
                    else if (cmp_ins (INS, LWU   )) { debug_info("LWU ");   ALU_RES = RS1_VAL + ext_s(IMM_I,11) ; PC=PC+4;   st = EXE_WR_TYPE_MEM2REG ;}

                    //else if (cmp_ins (INS, LD    )) { debug_info("LD ");    PC=PC+4;}

                    else if (cmp_ins (INS, SB    )) { debug_info("SB ");    ALU_RES=RS1_VAL+ext_s(IMM_S,11) ; PC=PC+4;  st = EXE_WR_TYPE_2MEM ;}
                    else if (cmp_ins (INS, SH    )) { debug_info("SH ");    ALU_RES=RS1_VAL+ext_s(IMM_S,11) ; PC=PC+4;  st = EXE_WR_TYPE_2MEM ;}
                    else if (cmp_ins (INS, SW    )) { debug_info("SW ");    ALU_RES = RS1_VAL+ext_s(IMM_S,11) ; PC=PC+4;  st = EXE_WR_TYPE_2MEM ;}
                    //else if (cmp_ins (INS, SD    )) { debug_info("SD ");    PC=PC+4;}

                    else if (cmp_ins (INS, ADDI  )) { debug_info("ADDI ");  ALU_RES = RS1_VAL + ext_s(IMM_I,11) ;   PC=PC+4; st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, ADDIW )) { debug_info("ADDIW "); ALU_RES = (U32_t)RS1_VAL + ext_s(IMM_I,11) ;   PC=PC+4;}

                    else if (cmp_ins (INS, ADD   )) { debug_info("ADD ");   ALU_RES = RS1_VAL + RS2_VAL ;  PC=PC+4;st = EXE_WR_TYPE_2REG ; } 
                    //else if (cmp_ins (INS, ADDW  )) { debug_info("ADDW ");  ALU_RES = (S32_t)RS1_VAL + (S32_t)RS1_VAL ;  PC=PC+4; }

                    else if (cmp_ins (INS, SUB   )) { debug_info("SUB ");   ALU_RES = RS1_VAL - RS2_VAL ;  PC=PC+4; st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SUBW  )) { debug_info("SUBW ");  ALU_RES = (S32_t)RS1_VAL - (S32_t)RS1_VAL ;  PC=PC+4; }

                    else if (cmp_ins (INS, XORI  )) { debug_info("XORI ");  ALU_RES = RS1_VAL ^ ext_s(IMM_I,11);  PC=PC+4; st = EXE_WR_TYPE_2REG ;}
                    else if (cmp_ins (INS, XOR   )) { debug_info("XOR ");   ALU_RES = RS1_VAL ^ RS2_VAL ;  PC=PC+4;st = EXE_WR_TYPE_2REG ;}

                    else if (cmp_ins (INS, ORI   )) { debug_info("ORI ");   ALU_RES = RS1_VAL | ext_s(IMM_I,11);   PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    else if (cmp_ins (INS, OR    )) { debug_info("OR ");    ALU_RES = RS1_VAL | RS2_VAL ;  PC=PC+4;st = EXE_WR_TYPE_2REG ;}

                    else if (cmp_ins (INS, ANDI  )) { debug_info("ANDI ");  ALU_RES = RS1_VAL &  ext_s(IMM_I,11) ;   PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    else if (cmp_ins (INS, AND   )) { debug_info("AND ");   ALU_RES = RS1_VAL & RS2_VAL ;  PC=PC+4;st = EXE_WR_TYPE_2REG ;}

                    else if (cmp_ins (INS, SLLI  )) { debug_info("SLLI ");  ALU_RES = RS1_VAL << ext_s(IMM_I,11);PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SLLIW )) { debug_info("SLLIW "); ALU_RES = RS1_VAL<< ext_s(IMM_I,11);PC=PC+4;}

                    else if (cmp_ins (INS, SLL   )) { debug_info("SLL ");   ALU_RES = RS1_VAL<<(RS2_VAL&31);PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SLLW  )) { debug_info("SLLW ");  ALU_RES = RS1_VAL<<RS2_VAL;PC=PC+4;}

                    
                    else if (cmp_ins (INS, SLTI  )) { debug_info("SLTI ");  ALU_RES = (int)RS1_VAL <  (int)ext_s(IMM_I,11); PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    
                    else if (cmp_ins (INS, SLT   )) { debug_info("SLT ");   ALU_RES = ((int)RS1_VAL <(int) RS2_VAL)?1:0; PC=PC+4;st = EXE_WR_TYPE_2REG ;}

                    else if (cmp_ins (INS, SLTU  )) { debug_info("SLTU ");  ALU_RES = (RS1_VAL < RS2_VAL)?1:0; PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    else if (cmp_ins (INS, SLTIU )) { debug_info("SLTIU "); ALU_RES = RS1_VAL >>  ( IMM_I & 31); PC=PC+4;st = EXE_WR_TYPE_2REG ;}

                    else if (cmp_ins (INS, SRLI  )) { debug_info("SRLI ");  ALU_RES = RS1_VAL >> ( IMM_I & 31);PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SRLIW )) { debug_info("SRLIW "); ALU_RES = RS1_VAL >>  IMM_I;PC=PC+4;}

                    else if (cmp_ins (INS, SRL   )) { debug_info("SRL ");   ALU_RES = RS1_VAL >> RS2_VAL;PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SRLW  )) { debug_info("SRLW");   ALU_RES = RS1_VAL >> RS2_VAL;PC=PC+4;}

                    else if (cmp_ins (INS, SRAI  )) { debug_info("SRAI ");  ALU_RES =(int) RS1_VAL >> ( IMM_I & 31);PC=PC+4;st = EXE_WR_TYPE_2REG ;}
                    //else if (cmp_ins (INS, SRAIW )) { debug_info("SRAIW "); ALU_RES = RS1_VAL >>  IMM_I;PC=PC+4;}

                    else if (cmp_ins (INS, SRA   )) { debug_info("SRA ");   ALU_RES =(int)RS1_VAL>>(RS2_VAL&31); PC=PC+4; st = EXE_WR_TYPE_2REG ; }
                    //else if (cmp_ins (INS, SRAW  )) { debug_info("SRAW ");  t=RS1_VAL>>RS2_VAL; ALU_RES = ext_s(t,(31-RS2_VAL));PC=PC+4;}

                    else {printf("PC = %08x Illegal ins\n",PC); exit(1);}break;
            case EXE_WR_TYPE_MEM2REG : LD_MEM();st=EXE_WR_REG_FROM_MEM;break;
            case EXE_WR_REG_FROM_MEM: WR_REG_FROM_MEM();st=EXE_WR_TYPE_NOP;break;
            case EXE_WR_TYPE_2REG: WR_REG_FROM_ALU();st=EXE_WR_TYPE_NOP;break;
            case EXE_WR_TYPE_2MEM: WR_MEM();st=EXE_WR_TYPE_NOP;break;
            case EXE_WR_TYPE_NOP: st = FETCH ; break;
            default : st = INIT ;
        }
    }
}
