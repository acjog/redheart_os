#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <string.h>

#define LOAD_BASE   0x30000
#define CODE_OFFSET 0x200 

char buf[4*1024];
  
// build函数主要负责将kernle.bin mm fs init 等连接起来 并且按4k对齐的方式
int main( int argc , char **argv )
{
	int i, j, m, n, k, u;
	int code_offset;
	int load_base;  // 加载进内存的地址  
	int seg_base[100], prog_base[5], size[5],  memsize, filesize, image_base;
	Elf32_Ehdr  fhead ;
	Elf32_Phdr  fload;
	FILE *fin , *fout ; 
	if ( argc < 6 )
	{
		printf("Usage: %s kernel.bin mm fs init\n",argv[0]);
		return 0;
	}

	char *p = "hello world";
	//  加载进内存的地址 
	load_base    = LOAD_BASE; 
	code_offset  = CODE_OFFSET;
	//  将镜像初值置为0
	image_base  = 0;
	prog_base[0] = 0 + load_base;
	// k 为记录段的个数 这里从1开始也是有好处的
	k  = 1;
	fout = fopen(argv[5] , "wb+");
	memset(buf, 0, sizeof( buf ));
	//  这里写一个4k作为预留信息 
	//  主要记录各个段要加载的偏移地址  
	if (NULL == fout) 
	{
		printf( "could not create out file\n" );
		exit(0);
	} 

	// 循环读取每个可执行文件头 以便连接起来
	for (i = 1; i < 5; i++) 
	{
		fin = fopen(argv[i], "rb");
		if ( NULL == fin )
		{
			printf("%s file open failed\n",argv[i]);
			exit(0);
		}
		//总共有4个程序 里面有5值是有好处的因为每次程序段都要根据前面的程序基址
		fread(&fhead, sizeof(Elf32_Ehdr), 1, fin);
		//要连接的段  
		n  = fhead.e_phnum;
		//紧接文件头为程序头段 同时对于gnu的栈段现在不加载
		n--;
		//该程序开始的段在程序的偏移首地址  
		seg_base[k]  = prog_base[i-1];
		k++;
		size[i] = 0;
		for ( m = 0; m < n; m++) 
		{
			//  偏移到当前程序头的位置
			fseek(fin , sizeof(Elf32_Ehdr)+m*sizeof( Elf32_Phdr) , SEEK_SET);
			//  读取程序头结构
			fread(&fload , sizeof( Elf32_Phdr ), 1, fin);
			memsize = (fload.p_memsz + 0xfff)&0xfffff000;//4k对齐
			image_base = image_base + memsize;
			filesize  = (fload.p_filesz  + 0xfff) & 0xfffff000;
			j = filesize>>12;        // 总共有多少个块
			filesize  = fload.p_filesz; 
			printf("filesize : 0x%x \n", filesize  );
			//  这个程序头在文件中的偏移  
			fseek(fin, fload.p_offset, SEEK_SET);
			size[i] +=  memsize >> 12;
			for ( u = 0; u < j; u ++) 
			{
				seg_base[k] =  seg_base[k-1] + 0x1000 ;
				k++;  
				memset( buf , 0 , sizeof ( buf ));
				//   是否为最后一个 
				if (j -1 == u)
				{
					fread (buf, filesize, 1, fin);
				}
				else
				{
					fread (buf, sizeof (buf), 1,fin);
				}
				filesize  -= 0x1000; 
				fwrite(buf ,sizeof(buf), 1, fout);
			}
			// 下一个段加上这个段的大小  
		}
		k--;
		prog_base[i] = image_base + load_base;
		fclose(fin);
	}

	//  跳过前面两个字节的jmp指令
	fseek(fout, code_offset + 8 , SEEK_SET );
	printf ("pro base :"); 
	for ( i = 0; i < 4; i++)
	{
		fwrite(&prog_base[i], sizeof (int), 1, fout);
		fwrite(&size[i+1], sizeof(int), 1, fout);
		printf(" 0x%x ", prog_base[i]);
	}
	printf("\nsize:  ");
	for (i = 1; i < 5; i++) 
	{
		printf("0x%x ",size[i]);
	}
	printf("\nsegment base :");
	for (i = 1 ; i < k ; i ++) 
	{
		fwrite( &seg_base[i], sizeof(int), 1, fout);
		printf("0x%x  ",seg_base[i]);
	}
	printf("\n");
	fclose(fout);
	return 0 ;
}
