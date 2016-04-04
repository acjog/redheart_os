#ifndef _STDARG_H_
#define _STDARG_H_

typedef  u8_t *va_list  ; //定义va_list为一个字符指针类型

// 依据类型计算偏移
#define __va_rounded_size(TYPE)\
	 ( ( sizeof(TYPE) + sizeof(int) - 1 )/sizeof(int) * sizeof(int))

// 根据最后一个参数 
#define   va_start(AP , LASTARG )       \
	( AP = ( u8_t *)&LASTARG + __va_rounded_size(LASTARG) )

#define  va_end( AP ) 
//  每次调用它 得到参数  一般调用它 ， 会用到一个赋值语句 
//  所以最后逗号分隔 其实就是赋值过去的
#define   va_arg(AP,TYPE)   \
	( AP += __va_rounded_size(TYPE) , \
	  *((TYPE *)( AP -__va_rounded_size(TYPE))))

#endif
