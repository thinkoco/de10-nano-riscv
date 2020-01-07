unsigned int get_ins_func7_b(unsigned int i){ 
	unsigned int r,mask ;
	mask = (1<<31)|(1<<30)|(1<<29)|(1<<28)|(1<<27)|(1<<26)|(1<<25);
	r = mask & i ;  
	return r;
}
	
unsigned int get_ins_func3_b(unsigned int i){
	unsigned int r,mask ;
	mask=0;
	mask = (1<<14)|(1<<13)|(1<<12);
	r = mask & i  ;  
	return r;	
}

unsigned int get_ins_opcode_b(unsigned int i ){
	unsigned int r,mask ;
	mask = (1<<2) | (1<<3)| (1<<4) | (1<<5) | (1<<6);
	r = mask & i ;  
	return r;
} 

unsigned int get_ins_func7_s(const unsigned char *p){
	unsigned int r=0;
	if (p[4]=='x')return -1;
		r=0;  if (p[4]=='1')r=1;
	r<<=1;if (p[5]=='1')r|=1;
	r<<=1;if (p[6]=='1')r|=1;
	r<<=1;if (p[7]=='1')r|=1;
	r<<=1;if (p[8]=='1')r|=1;
	r<<=1;if (p[9]=='1')r|=1;
	r<<=1;if (p[10]=='1')r|=1;
	r<<=25;		
	//printf("                              func7 result is %08x\n",r);
	return r ;
}
	
unsigned int get_ins_func3_s(const unsigned char *p){
	unsigned int r=0;
	if (p[12]=='x')return -1;
	r=0  ;if (p[12]=='1')r|=1;
	r<<=1;if (p[13]=='1')r|=1;
	r<<=1;if (p[14]=='1')r|=1;
	r<<=12;
	return r;
}

unsigned int get_ins_opcode_s(const unsigned char *p){
	unsigned int r=0;
	if (p[16]=='x')return -1;
	r=0  ;if (p[16]=='1')r|=1;
	r<<=1;if (p[17]=='1')r|=1;
	r<<=1;if (p[18]=='1')r|=1;
	r<<=1;if (p[19]=='1')r|=1;
	r<<=1;if (p[20]=='1')r|=1;
	r<<=2;
	return r;
}

unsigned int get_bits(unsigned int v,unsigned int left,unsigned int right){
	 int i ;
	 unsigned int r = 0 ;    
	 for(i=left;i>=right;i--){
		 r<<=1;  
		 if ( v &(1<<i)) 
			r|=1;
	 } 
	 return r;
}

		
int cmp_ins(unsigned int src_ins,const char * dst_ins){
	int r ;  
	unsigned   int src_opcode,src_func3,src_func7;
	unsigned   int dst_opcode,dst_func3,dst_func7;
	dst_opcode = get_ins_opcode_s(dst_ins) ;
	dst_func3 = get_ins_func3_s(dst_ins) ;
	dst_func7 = get_ins_func7_s(dst_ins) ; 
	src_opcode = get_ins_opcode_b(src_ins) ;
	src_func3 = get_ins_func3_b(src_ins) ;
	src_func7 = get_ins_func7_b(src_ins) ; 
	if ((dst_opcode != -1) &&( dst_opcode  != src_opcode)) return 0;
	if ((dst_func3 != -1) &&( dst_func3  != src_func3)) return 0;
	if ((dst_func7 != -1) &&( dst_func7  != src_func7)) return 0;
	return 1 ;
}


unsigned int ext_s (unsigned int val ,unsigned int hi_bit){
         int i ; 
		 unsigned int r = 0 ;
		 unsigned int flag_bit;
		 flag_bit = (val & (1<<hi_bit) )?1:0;
         for(r=0,i = 31;i>hi_bit;i--){
		 	 if (flag_bit) r |= 1 << i ;
			 else r &= ~(1<<i) ;
         }
         for(i = hi_bit;i>=0;i--){
		 	 if (val & (1<<i)) r |= 1 << i ;
			 else r &= ~(1<<i) ;
         }
         return r;
}

