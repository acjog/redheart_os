#ifndef  __TTY_H__
#define  __TTY_H__


//  特殊标记  相当于ctrl+c
#define MARKER    0  
#define RAW       1
#define COOKED    2

#define NOT_SUSPENDED 0
#define SUSPENDED     1

#define  TTY_NR              4
#define  TTY_IN_BYTES        256
#define  TTY_RAM_WORDS       320
#define  TTY_BUF_SIZE        128
#define  CURRENT_TTY         0
#define  DRIVER_BUF_SIZE     16 

#define  LINE_WIDTH          80
#define  SCR_LINES           25 


EXTERN struct tty_line{
     //  终端缓冲队列 
     u8_t    *tty_inhead               ;
     u8_t    *tty_intail               ;
     i32_t   tty_incount               ;
     u8_t    tty_inqueue[TTY_IN_BYTES] ;
     i32_t   tty_lfct                  ;//  输入队列已经形成多少行了 主要区分原始和杜撰模式
     //  终端显示视频
     u16_t    tty_outqueue[TTY_RAM_WORDS] ; //此处的一次性输出tty_outwords个字符
     u16_t   tty_outwords              ;  //输出缓冲的个数
     i32_t   tty_org                   ;  //显示在显存的首地址
     u32_t   tty_vid                   ;  //光标的位置
     u16_t    tty_attr                 ;  //字符属性
     i32_t    (* tty_devstart)(/*struct tty_line **/);  //终端实际输出函数
     //终端参数
     u8_t   tty_mode       ;
     u8_t    tty_column     ;  //当前列
     u8_t    tty_row        ;  //当前行
     u8_t    tty_waiting    ;    
     u8_t    tty_inhibited  ;
     // 读终端部分
     // 与进程相关的
     i32_t   tty_incaller   ;    // 一般为FS系统
     i32_t   tty_inproc     ;    // 执行此调用的进程
     u8_t    *tty_in_laddr  ;    // 该进程的线性地址
     u32_t   tty_inleft     ;
     // 向终端写
     i32_t   tty_outcaller  ;    //  一般FS文件系统
     i32_t   tty_outproc    ;    // 执行调用此进程的
     u8_t   *tty_out_laddr  ;    //调用此调用的进程
     u32_t   tty_outleft    ;
     u32_t   tty_cum        ;   // chars拷贝到tty_outqueue 
}tty_struct[TTY_NR];

//本文件内部使用的函数
void print_proc();

//处理char  interrupt相关函数
i32_t  do_char_int()                 ;
void   in_char( u8_t , u8_t )        ;
u8_t   make_break( u8_t )            ;
void   out_char( struct tty_line* , u8_t )             ;
void   move_to( struct tty_line * , i32_t , i32_t )    ;
void   flush( struct tty_line *tp )                    ;
void   chuck( struct tty_line *)                       ;
void   echo( struct tty_line* , u8_t  )                ;
void   init_tty()                                      ;
i32_t  printk( u8_t * , ... )                          ;

//tty 读写函数
void  do_read(struct tty_line *tp , message   *);
void  do_write(struct tty_line *tp , message  *);
void  do_cancel(struct tty_line *tp , message *);
void  do_ioctl(struct tty_line *tp , message  *);
void  tty_reply(i32_t,i32_t , i32_t,i32_t,i32_t,i32_t);

i32_t   rd_chars(struct tty_line *tp );
void    console( struct tty_line *tp );
u8_t	get_byte(i32_t caller, u8_t* vir_addr  );
void    finish(struct tty_line *tp , i32_t num );

//定义在tty.c的全局变量
EXTERN  u32_t current_tty ;

#endif
