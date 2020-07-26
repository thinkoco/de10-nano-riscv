#include <stdio.h>
#include <stdint.h>
#define BUFF_SIZE 1024*64*2
static unsigned int ins_buff[BUFF_SIZE] ; 
static unsigned int len = 0 ;

int file2ins_buff(char *fn){
	FILE *fp ; 
	fp = fopen( fn , "rb" );	
	if (fp==NULL){
		printf("can not open file %s\n",fn);
		return 0;
	}
	len = fread( ins_buff , 1, BUFF_SIZE ,fp );
	fclose (fp);
	printf("binary length: %d bytes\n", len);
	return len ;	
}

int main(int argc, char ** argv)
{
    FILE *fp1;
	file2ins_buff(argv[1]);
    fp1 = fopen("e200_riscv_itcm.mif", "w+");
	int i = 0;
	fprintf(fp1, "WIDTH = 64;\nDEPTH = 1024;\n"
				 "ADDRESS_RADIX = HEX;\n"
				 "DATA_RADIX = HEX;\n"
				 "CONTENT "
				 "BEGIN\n");
	for(i = 0; i < len/4; i = i+2){
		fprintf(fp1, "	%04X : %08X%08X;\n", i/2, ins_buff[i+1],ins_buff[i]);
	}
	if((len%8 >0) && (len%8 < 4)){
        i = (len/4);
		fprintf(fp1, "	%04X : %08X%08X;\n", i/2, ins_buff[i+1],ins_buff[i]);
	}
	fprintf(fp1, "END;\n");
	fclose(fp1);

	printf("e200_riscv_itcm.mif generated \n");
}
