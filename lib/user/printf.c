#include <type.h>
#include <stdarg.h>

//外部函数声明
extern void putc(u8_t );
// 这是用户态的程序
#define flush    _flush 
//内部函数声明
void itoa(i32_t ,char *);

i32_t printf( u8_t *fmt , ... );

//所用到的缓冲
u8_t   tmp[18] ;

void itoa(i32_t num,  char* str)
{
		//00b800h  - > 'b800'
 	i32_t	mask = 32, i = 0;
 	char	value,index=0;
 	for( i=0; i<=7; i++)
 	{
 		mask-=4;
 		value= (num >> mask )&0xf;
 		if ( 0 == value && 0 == index)
 		{
 			continue;
 		}
 		else if( value > 9)
 		{
 			str[index]=value-10+'A';
 			index++;
 		}
 		else if( value <= 9)
 		{
 			str[index]=value+'0';
 			index++;
 		}
 	}
	str[index] = 0 ;
 	return ;
}
// 格式化输出函数 
i32_t printf( u8_t *fmt , ... )
{
	u8_t *psrc ,*p;
	i32_t  a , i  ;
	va_list ap ;
	va_start(ap,fmt) ;
	psrc  = fmt ;
	for (    ; *psrc != 0  ;  )
	{
            if ( *psrc != '%' ) 
	    {
		    putc(*psrc++);
		    continue ;
	    }
	    psrc++;
        switch ( *psrc++ )
	    {
	       case 'd':
	       {
                a =  va_arg(ap,i32_t) ;
		        tmp[0] = '0' ;
		        tmp[1] = 'x' ;
                itoa( a , tmp+2 );
                i  = 0 ;
                while ( tmp[i] != 0 ) 
                {
                    putc(tmp[i++]);
                }
                break ; 
	        }
	        case 's':
		    {
                 p = va_arg( ap , u8_t *) ; 
                 while ( *p != 0  )
                 {
                    putc(*p++);
                 }	 
                 break ;
	        }
	        default :
	                 return 1 ;// 错误发生
	    }
	}
	va_end ( ap );
	flush();
	return  0 ;
}
