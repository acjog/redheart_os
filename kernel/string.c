
//这些东西全部直接照抄linux 我没有对它进行任何不必要的修改 我的修改也没有必要

// 字符串拷贝函数 直到遇到NULL结束
//参数：dest - 目的字符串指针 src - 源字符串指针 
char* strcpy(char *dest , const char *src )
{
  __asm__("cld\n"                     //清方向位
	  "1:\tlodsb\n\t"             //加载ds:esi处的1字节->al 并更新esi
	  "stosb\n\t"	              //存储al->es:edi 并更新edi
	  "testb %%al , %%al\n\t"     // 测试刚存储的字符是否为0
	  "jne  1b"                   // 不是则后跳到标号1处，否则结束
	  ::"S"(src),"D"(dest)); 
  return dest ;
}

// 拷贝源字符串count个字节到目的字符串
// 如果源串长度小于count个字节，就附加到空字符到目的字符串
// 参数：dest - 目的字符串指针 src - 源字符串指针 count - 拷贝字节数
// %0 - src %1 - dest %2 - count 
char * strncpy(char *dest , const char *src , int count )
{
  __asm__("cld\n"                    // 清方向位
	  "1:\tdecl %2\n\t"          // 寄存器ecx--(count--)
	  "js 2f\n\t"                // 如果count<0则跳到标号2，结束
	  "lodsb\n\t"                // 取ds:esi -> al ， esi++
	  "stosb\n\t"                //  al ->  es:edi , edi++
	  "testb %%al , %%al\n\t"    //  该字节为0
	  "jne  1b\n\t"              // 不是，则向前跳到标号1处，继续复制
	  "rep\n\t"                  // 否则 存放剩余个数的字节数
	  "stosb\n"
	  "2:"
	  ::"S"(src),"D"(dest),"c"(count)
		  );
  return dest ;
}
//
// %0 - esi(src)  %1 - edi(dest)   %2 -eax(0) %3 - ecx(-1)
char* strcat(char *dest, const  char *src  )
{
  __asm__("cld\n\t"
	  "repne\n\t"
	  "scasb\n\t"
	  "decl  %1\n"
	  "1:\tlodsb\n\t"
	  "stosb\n\t"
	  "testb  %%al, %%al\n\t"
	  "jne  1b"
	  ::"S"(src),"D"(dest),"a"(0),"c"(0xffffffff)
		  );
  return dest ;
}

//
// %0 - src %1 - dest %2 - eax %3 - ecx %4 - count 
char * strncat(char *dest , const char *src , int count )
{
  __asm__("cld\n\t"
          "repne\n\t"
	  "scasb\n\t"
	  "decl  %1\n\t"
	  "movl  %4,%3\n"
	  "1:\tdecl %3\n\t"
	  "js 2f\n\t"
	  "lodsb\n\t"
	  "stosb\n\t"
	  "testb  %%al , %%al\n"
	  "jne  1b\n"
	  "2:\txorl  %2,%2\n\t"
	  "stosb"
	  ::"S"(src),"D"(dest),"a"(0),"c"(0xffffffff),"g"(count)
		  );
  return dest ;
}

// 将字符串1与字符串2进行比较
// 参数： cs - 字符串1 ct - 字符串2 
// %0 - eax(_res) %1 - edi(cs)字符串1指针  %2 - esi(ct)字符串2指针
// 返回：如果串1 》 串2 ， 则返回1；串1=串2，则返回0，串1《串2，则返回-1
int strcmp(const  char *cs , const char *ct)
{
 register int __res __asm__("ax");
 __asm__("cld\n"
         "1:\tlodsb\n\t"
	 "scasb\n\t"
	 "jne 2f\n\t"
	 "testb  %%al,%%al\n\t"
	 "jne 1b\n\t"
	 "xorl  %%eax , %%eax\n\t"
	 "jmp 3f\n"
	 "2:\tmovl $1,%%eax\n\t"
	 "jl  3f\n\t"
	 "negl %%eax\n"
	 "3:"
	 :"=a"(__res):"D"(cs),"S"(ct)
		 );
 return __res;
}

//  字符串1与字符串2比较前count个字符
//  参数: cs - 字符串1 , ct - 字符串2 count - 比较字符数
//  %0 - eax(_res)返回值 %1 - edi(cs)串1指针  %2 - esi(ct)串2指针  %3 - ecx 
int strncmp(const  char * cs , const char * ct , int count )
{
   register  int __res __asm__("ax");
   __asm__("cld\n"
	   "1:\tdecl  %3\n\t"
	   "js  2f\n\t"
	   "lodsb\n\t"
	   "scasb\n\t"
	   "jne  3f\n\t"
	   "testb  %%al , %%al\n\t"
	   "jne  1b\n"
	   "2:\txor  %%eax , %%eax\n\t"
	   "jmp  4f\n\t"
	   "3:\tmovl  $1 , %%eax\n\t"
	   "jl  4f\n\t"
	   "negl  %%eax\n"
	   "4:"
	   :"=a"(__res):"D"(cs),"S"(ct),"c"(count)
		   );
   return __res;
}

//计算字符串长度。参数s -字符串
//
int strlen(const char *s )
{
  register int __res ; 
  __asm__("cld\n\t"
	  "repne\n\t"
	  "scasb\n\t"
	  "notl  %0\n\t"
	  "decl  %0"
          :"=c"(__res):"D"(s),"a"(0),"c"(0xffffffff)
		  );
  return __res;
}

// 内存块复制 从源地址src处开始复制n个字符到目的地址dest处
// 参数:  dest - 复制目的地址  src - 复制的源地址  n - 复制字节数 
void *memcpy(void *dest , const void *src , int n )
{
   __asm__("cld\n\t"
	   "rep\n\t"
	   "movsb"
	   ::"c"(n),"S"(src),"D"(dest)
		   );
   return dest ; 
}

//用指定字符c填写指定长度count内存块s
//%0 - eax %1 - edi %2 - esi 
void *memset(void *s , char c , int count )
{
  __asm__("cld\n\t"
	  "rep\n\t"
	  "stosb"
	  ::"a"(c),"D"(s),"c"(count)
		  );
  return s ;
}
