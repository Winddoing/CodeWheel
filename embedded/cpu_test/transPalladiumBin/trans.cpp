#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>
//	row_count = 14,cs,bankcount = 3,cscount = 1;
class Translate{
private:
	int sAddr;
	int rowCount,bankCount,csCount,colCount;
	uint32_t *readBuf;
	FILE *qelFile;
	uint32_t writeHexFile(unsigned int addr,uint32_t *data,int count)
	{
		FILE *hifp,*lofp;
		char filename[256];
		unsigned short hisum = 0;
		unsigned short losum = 0;
		snprintf(filename,256,"kernelhex/hi%08x.txt",addr);
		hifp = fopen(filename,"w+t");
		if(hifp == NULL)
		{
			printf("generate %s failure.err:%s\n",filename,strerror(errno));
			exit(-1);
		}
		snprintf(filename,256,"kernelhex/lo%08x.txt",addr);
		lofp = fopen(filename,"w+t");
		if(lofp == NULL)
		{
			printf("generate %s failure.err:%s\n",filename,strerror(errno));
			exit(-1);
		}
		for(int i = 0;i < count;i++){
			unsigned short dat = data[i] & 0xffff;
			losum |= dat;
			fprintf(lofp,"%04x\n",dat);
			dat = (data[i] >> 16) & 0xffff;
			hisum |= dat;
			fprintf(hifp,"%04x\n",dat);
		}
		fclose(hifp);
		fclose(lofp);
		return losum | (hisum << 16);
	}
	void translate(unsigned int addr,uint32_t *data,int count){
		unsigned int rowlo,rowhi,raddr,cs,bank;
		cs = (addr >> (colCount + rowCount + bankCount)) & ((1 << csCount) - 1);
		bank = (addr >> (colCount + rowCount)) & ((1 << bankCount) - 1);
		rowlo = (addr >> colCount) & ((1 << bankCount) - 1);
		rowhi = (addr >> (colCount + bankCount)) & ((1 << (rowCount - bankCount)) - 1);
		unsigned int cpuaddr = (cs << (rowCount + colCount + bankCount)) | (rowhi << (bankCount + colCount)) | (bank << colCount) | (rowlo << (rowCount + colCount));
		writeHexFile(cpuaddr,data,count);
		fprintf(qelFile,"# addr:0x%08x cs:0x%x bank:0x%x rowhi:0x%x rowlo:0x%x transaddr:0x%08x\n",addr,cs,bank,rowhi,rowlo,cpuaddr);
		fprintf(qelFile,"memory -load -start %d %%readmemh itb_ddr0.ddr3_chip%d_lo.memcore -file ./data/kernelhex/lo%08x.txt -bg\n",cpuaddr / 4,cs,cpuaddr);
		fprintf(qelFile,"memory -load -start %d %%readmemh itb_ddr0.ddr3_chip%d_hi.memcore -file ./data/kernelhex/hi%08x.txt -bg\n",cpuaddr / 4,cs,cpuaddr);
	}
public:
	Translate(int saddr,int row,int bank,int cs,int col,int bitwidth){
		char s[256];

		rowCount = row;
		if(bank == 8)
			bankCount = 3;
		else if(bank == 4)
			bankCount = 2;
		else {
			printf("Bank %d not support!\n",bank);
			exit(-1);
		}
		if(cs == 2)
			csCount = 1;
		else if(cs == 1)
			csCount = 0;
		else {
			printf("cs %d not support!\n",cs);
			exit(-1);
		}
		int w = 16;
		if(bitwidth == 16)
			w = 1;
		else if(bitwidth == 32)
			w = 2;
		else{
			printf("bitwidth %d not support!\n",bitwidth);
			exit(-1);
		}
		if(col + w >= 12)
			colCount = col + w;
		else {
			colCount = 12;
			rowCount -= 12 - col + w;
		}
		readBuf = new uint32_t[(1 << colCount) >> 2];
		sAddr = saddr;
		const char *qelFileName = "s.qel";
		qelFile = fopen(qelFileName,"wt");
		if(qelFile == NULL)
		{
			printf("generate %s failure.err:%s\n",s,strerror(errno));
			exit(-1);
		}
	}
	int write(FILE *f){
		int size = (1 << colCount);
		memset(readBuf,0,size);
		int addr = sAddr;
		if((addr & (size  - 1)) != 0) {
			printf("Address[0x%08x] should be col aligned!\n",addr);
		}
		int readlen = 0;
		while(!feof(f)){
			int len = fread(&readBuf[readlen],1,size - readlen,f);
			readlen += len;
			if(readlen == size){
				translate(addr,readBuf,readlen / 4);
				addr += size;
				readlen = 0;
			}else if(len == 0)
				break;
		}
		if(readlen)
			translate(addr,readBuf,readlen / 4);
	}
	~Translate(){
		delete[] readBuf;
		fclose(qelFile);
	}
};
void help(char **argv)
{
	fprintf(stderr, "Usage: %s -c cs -b bank -r row -l col -w bitwidth -a startaddress -f fileName\n",
			argv[0]);
	fprintf(stderr, "-c ddr cs count.\n");
	fprintf(stderr, "-b ddr bankcount.\n");
	fprintf(stderr, "-r ddr raw.\n");
	fprintf(stderr, "-l ddr column.\n");
	fprintf(stderr, "-l ddr column.\n");
	fprintf(stderr, "-a startaddress.\n");
	fprintf(stderr, "-f handle translate addr filename\n");
}
int main(int argc, char *argv[])
{
	int cs = 0,row = 0,bitwidth = 0,col = 0,bank = 0;
	unsigned int startaddr;
	char *fileName;
	int opt;
	while ((opt = getopt(argc, argv, "c:b:w:r:l:a:f:")) != -1) {
		switch (opt) {
		case 'c':
			cs = atoi(optarg);
			break;
		case 'b':
			bank = atoi(optarg);
			break;
		case 'w':
			bitwidth = atoi(optarg);
			break;
		case 'r':
		    row = atoi(optarg);
			break;
		case 'l':
		    col = atoi(optarg);
			break;
		case 'a':
			startaddr = (unsigned int)strtoul(optarg,0,0);
			break;
		case 'f':
			fileName = optarg;
			break;
		default: /* '?' */
			help(argv);
			return -1;
		}
	}
	if (optind > argc) {
	 	fprintf(stderr, "Expected argument after options %d %d\n",optind,argc);
		return -1;
	}
	if(cs == 0 && bank == 0 && col == 0 && row == 0 && bitwidth == 0){
		help(argv);
		return -1;
	}
	printf("Filename %s\n",fileName);
	printf("ddr cs:%d bank:%d row:%d col:%d bitwidth:%d\n",cs,bank,row,col,bitwidth);
	printf("ddr file start address 0x%08x\n",startaddr);
	FILE *fp;
	fp = fopen(fileName,"rb");
	if(fp == NULL){
		printf("input file[%s] open failure.:%s\n",fileName,strerror(errno));
		exit(-1);
	}
	Translate *t = new Translate(startaddr,row,bank,cs,col,bitwidth);
	t->write(fp);
	delete t;
	fclose(fp);
    return 0;
}
