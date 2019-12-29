#include "stdio.h"
#define BUFF_SIZE 1024*64*2
static unsigned int ins_buff[BUFF_SIZE] ; 
static unsigned int len = 0 ;
static unsigned int debug = 0 ;

int file2ins_buff(char *fn){
	int r,i ;	FILE *fp ; 
	fp = fopen( fn , "rb" );	
	if (fp==NULL){
		printf("can not open file %s\n",fn);
		return 0;
	}
	r = fread( ins_buff , 1, BUFF_SIZE ,fp );
	fclose (fp);
	len = r>>2;
	if (debug){
		printf("get file size is %d\n", len );
		for( i = 0 ;i<len ;++i ){
             printf("%05x : %08x \n",i<<2, ins_buff[i] );
        }
	}
	return r ;	
}

void word_endian_change(){
	int i ;
	unsigned char tmp0,tmp1,*p;
    p = (unsigned char *)ins_buff;
	for( i = 0 ;i<len*4 ;i=i+4 ){
		tmp0 = p[i];
		tmp1 = p[i+1];
		p[i]   = p[i+3];
		p[i+1] = p[i+2];
		p[i+2] = tmp1 ;
		p[i+3] = tmp0 ;
	}
}

void print_intelhex(){
	int i ;
	unsigned char bycount     = 8;
	unsigned char record_type = 0;
	unsigned char checksum    = 0;
	unsigned char *data;
	unsigned char addr[2];
	for( i = 0 ;i<len ;i=i+2 ){
		addr[0] = (i>>1) & 0xFF;
		addr[1] = (i>>5) & 0xFF;
		data = (unsigned char *)(ins_buff+i);
		checksum = bycount + record_type + addr[0] + addr[1] + \
					data[0] + data[1] + data[2] + data[3] + data[4] + \
					data[5] + data[6] + data[7];
		checksum = ~checksum;
		checksum ++;
		if(debug)
			printf("bc:%02X addr:%04X record_type:%02X data:%08x%08x checksum:%02X\n",bycount,i>>1,record_type,ins_buff[i+1],ins_buff[i],checksum);
		else
			printf(":%02X%04X%02X%08X%08X%02X\n",bycount,i>>1,record_type,ins_buff[i+1],ins_buff[i],checksum);
	}
	printf(":00%04X01FF\n",0); //the intel hex end
}

int main (int argc, char **argv){
	if(argc >1){
		debug = argv[2] ? 1:0;
		file2ins_buff(argv[1]);
		//word_endian_change();
		print_intelhex();
	}else {
		printf("usage :  bin2mem x.bin\n");
	}
	return 0;
}
