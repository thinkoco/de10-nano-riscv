static unsigned int ins_buff[ITCM_SIZE] ; 
static unsigned int len = 0 ;
static unsigned char RAM_B0[ITCM_SIZE] ; 
static unsigned char RAM_B1[ITCM_SIZE] ; 
static unsigned char RAM_B2[ITCM_SIZE] ; 
static unsigned char RAM_B3[ITCM_SIZE] ;


static int file_2_ins_buff(char *fn){
	int r,i ;	FILE *fp ; 
	fp = fopen( fn , "rb" );	
	if (fp==NULL) printf("can not open file %s\n",fn);
	if (fp==NULL) exit(1);
	r = fread( ins_buff , 1, ITCM_SIZE ,fp );
	fclose (fp);
	len = r>>2;
	//printf("get file size is %d\n", len );
		for( i = 0 ;i<len ;++i ){
             debug_info("%08x : %08x \n",i<<2, ins_buff[i] );
             }
	return r ;	
}

static int init_MEM_by_ins_buff(){
	int i ;
	for( i = 0 ;i<len ;++i ){
		//debug_info("%08x :  ",ins_buff[i] );
		RAM_B0[i] = ins_buff[i] & 0xff ;
		RAM_B1[i] = ins_buff[i]>>8 & 0xff ;
		RAM_B2[i] = ins_buff[i]>>16 & 0xff ;
		RAM_B3[i] = ins_buff[i]>>24 & 0xff ; 
		debug_info("%08x : %02x  %02x  %02x  %02x \n",i<<2,RAM_B3[i],RAM_B2[i],RAM_B1[i],RAM_B0[i]);	
	}
}

int load_prog(char *fn )  {
    file_2_ins_buff(fn) ; 
    init_MEM_by_ins_buff();
}
 
unsigned int get_ins(unsigned int PC){
	unsigned int t;
	t =(PC-ITCM_ADDR) >>2 ;
	return (( RAM_B3[t] << 24) | ( RAM_B2[t] << 16) | (RAM_B1[t] <<8) | RAM_B0[t]);
} 

static inline unsigned int read_itcm_8bit(unsigned int addr){
	unsigned  int addr32,idx;
	addr32  =  (addr-ITCM_ADDR) >>2 ;
	idx = addr % 4 ;
	switch(idx){
		case 0  : return RAM_B0[addr32];
		case 1  : return RAM_B1[addr32];
		case 2  : return RAM_B2[addr32];
		default : return RAM_B3[addr32]; //3
	}
}

static inline void write_itcm_8bit (unsigned int val,unsigned int addr){
	unsigned char d8  ;
	unsigned  int addr32,idx;
	addr32  =  (addr-ITCM_ADDR) >> 2 ;
	idx = addr % 4 ;
	d8 = val &0xff ;
	switch(idx){
		case 0  : RAM_B0[addr32] = d8;
		case 1  : RAM_B1[addr32] = d8;
		case 2  : RAM_B2[addr32] = d8;
		default : RAM_B3[addr32] = d8; //3
	}
	//debug_info("                                      U8 IO WR: %08x ,%08x\n",addr,val);	
}

static inline unsigned int read_itcm_16bit(unsigned int addr){
	unsigned  int addr32,idx;
	unsigned char hi,lo;
	unsigned int r ; 
	addr32  =  (addr-ITCM_ADDR) >>2;
	idx = addr % 4 ;
	if      (idx==0) { hi = RAM_B1[addr32] ;lo =  RAM_B0[addr32]; }
	else if (idx==2) { hi = RAM_B3[addr32] ;lo =  RAM_B2[addr32] ; };
	r = hi ;	r<<=8;	r|=lo;
	return r ;
}

static inline void write_itcm_16bit (unsigned int val,unsigned int addr){
	unsigned  int addr32,idx;
	unsigned char hi,lo;
	unsigned int r ; 
	lo = val & 0xff;	val >>= 8 ;	hi = val& 0xff ;	addr32  =  (addr-ITCM_ADDR) >>2 ;	idx = addr % 4 ;
	if      (idx==0) { RAM_B1[addr32] = hi ; RAM_B0[addr32] = lo ;}
	else if (idx==2)  { RAM_B3[addr32] = hi ; RAM_B2[addr32] = lo ;}
	return   ;
}


static inline unsigned int read_itcm_32bit(unsigned int addr){
	unsigned  int addr32,idx; 
	unsigned int r ; 
	addr32  =  (addr - ITCM_ADDR) >> 2; 
	r = (( RAM_B3[addr32] << 24) | ( RAM_B2[addr32] << 16) | (RAM_B1[addr32] <<8) | RAM_B0[addr32]);
	debug_info("read memory[ %08x ]= %08x \n",addr,r);
	return r ;
}

static inline void write_itcm_32bit (unsigned int val,unsigned int addr){
	unsigned  int addr32,idx; 
	unsigned int t ;
	addr32  =  (addr-ITCM_ADDR) >> 2; 
	t = val ;
	RAM_B0[addr32] = t &0xff ; t>>=8;
	RAM_B1[addr32] = t &0xff ; t>>=8;
	RAM_B2[addr32] = t &0xff ; t>>=8;
	RAM_B3[addr32] = t &0xff ;

}


unsigned int read_memory_8bit(unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		return read_itcm_8bit(addr);
	}else{
		printf("[read_memory_8bit]: address is out of itcm!\n");
		return 0;
	}
}



void write_memory_8bit (unsigned int val,unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		write_itcm_8bit(val,addr);
	}else{
		printf("[write_memory_8bit]: address is out of itcm!\n");
	}
}



unsigned int read_memory_16bit(unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		return read_itcm_16bit(addr);
	}else{
		printf("[read_memory_16bit]: address is out of itcm!\n");
		return 0;
	}
}

void write_memory_16bit (unsigned int val,unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		write_itcm_16bit(val,addr);
	}else{
		printf("[write_memory_16bit]: address is out of itcm!\n");
	}
}


unsigned int read_memory_32bit(unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		return read_itcm_32bit(addr);
	}else{
		printf("[read_memory_32bit]: address is out of itcm!\n");
		return 0;
	}
}

void write_peripheral_32bit(unsigned int val,unsigned int addr)
{
	static unsigned char cnt = 0;
	static unsigned int gpio_ctl  = 0;
	switch(addr){
		case LED_OUT_CTRL : {printf("gpio ctrl     :%d\n",val);gpio_ctl = val; break;}
		case LED_OUT_VALU : {printf("gpio out value:%d",(val&gpio_ctl)); fflush(stdout); printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"); break;}
		case UART_VALU :{printf("%d <- uart",(val&0xff));fflush(stdout); printf("\b\b\b\b\b\b\b\b");cnt++; if(cnt == 32){cnt=0;printf("\n");} break;}; 
		default : printf("[write_peripheral_32bit]: there is no peripheral in 0x%08X\n",addr);
	}   
}

void write_memory_32bit (unsigned int val,unsigned int addr){
	if(( addr >= ITCM_ADDR) && ((addr-ITCM_ADDR)<=ITCM_SIZE)){
		write_itcm_32bit(val,addr);
	}else{
		write_peripheral_32bit(val,addr);
	}
}
 


int  write_memory_mask  (unsigned int val,unsigned int addr,unsigned int mask){
	unsigned int t ;
    		t = ins_buff[addr];
		if ( mask==0xf )	 ins_buff[addr] = val ;
        if ( mask & 0x1 )  { ins_buff[addr] &= 0xffffff00; ins_buff[addr] |= ( val &0x000000ff) ;   }
        if ( mask & 0x2 )  { ins_buff[addr] &= 0xffff00ff; ins_buff[addr] |= ( val &0x0000ff00) ;   }
        if ( mask & 0x4 )  { ins_buff[addr] &= 0xff00ffff; ins_buff[addr] |= ( val &0x00ff0000) ;   }
        if ( mask & 0x8 )  { ins_buff[addr] &= 0x00ffffff; ins_buff[addr] |= ( val &0xff000000) ;   }	
	return val ; 
}
 
