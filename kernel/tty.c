#include <type.h>
#include <protect.h>
#include <glo_fun.h>
#include <com.h>
#include "keyboard.h"
#include <stdarg.h>
#include <callnr.h>
#include <string.h>
#include <tty.h>
//使用到的变量

static u8_t    driver_buf[2*DRIVER_BUF_SIZE+2];   //硬件发送过来的原始字符
static u8_t    tty_buf[TTY_BUF_SIZE];             //用户进程读写终端
static u8_t    tty_copy_buf[2*DRIVER_BUF_SIZE];   //为避免竞态

static u8_t    l_shift = 0;
static u8_t    r_shift = 0;
static u8_t    l_ctrl  = 0;
static u8_t    r_ctrl  = 0;
static u8_t    l_alt   = 0;
static u8_t    r_alt   = 0;
static u8_t   cloumn  = 0;
static u8_t    e0      = 0;  //用来检测特殊功能键
static u8_t    up      = 0 ;
static u8_t    down    = 0 ;
static u8_t    left    = 0 ;
static u8_t    right   = 0 ;

//消息
static  message tty_mess;
//程序主流程
void  tty_task( )
{
   i32_t   r ; 
   struct  tty_line *tp ;
   init_tty();
   printf("tty_task ") ;
   while( TRUE )
   {
	receve( ANY , &tty_mess);
	tp  = &tty_struct[tty_mess.TTY_LINE];
	switch ( tty_mess.m_type )
	{
		case  CHAR_INT :
			{
				r =  do_char_int() ;
				break ;
			}
		case  TTY_READ:
			{
				do_read(tp,&tty_mess);
				break;
			}
		case TTY_WRITE:
			{
				do_write(tp,&tty_mess);
				break;
			}
		case  TTY_CANCEL:
			{
				do_cancel(tp,&tty_mess);
				break;
			}
		case  TTY_IOCTL:
			{
				do_ioctl(tp,&tty_mess);
				break;
			}
		default:
			{
			  printf( " tty coming ... " );
			  break;
			}

	}
   }
}

void init_tty()
{
    u8_t   i ;
    //  底层的设备驱动缓冲
    current_tty       = CURRENT_TTY    ;
    driver_buf[0]     = 0 ;
    driver_buf[1]     = DRIVER_BUF_SIZE ;
    //  当前终端的缓冲  我们仅有4个终端 每个8k 0x2000
    for ( i = 0 ; i < TTY_NR ; i++ )
    {
      tty_struct[i].tty_org     =   i * 0x2000  ;
      if ( i != 0  ) 
      {
          tty_struct[i].tty_vid     =   0  ;
      }
      tty_struct[i].tty_mode       =   COOKED ; // 杜撰模式 
      tty_struct[i].tty_lfct       =   0      ; // 还没有积累一行
      tty_struct[i].tty_inhead     =   tty_struct[i].tty_inqueue;
      tty_struct[i].tty_intail     =   tty_struct[i].tty_inqueue;
      Memset(tty_struct[i].tty_inqueue,TTY_IN_BYTES);
      tty_struct[i].tty_incount    =    0;
      tty_struct[i].tty_devstart   =    console ;
      // 这里原minix代码是每次quit或exit操作后将
      // 其改为RUNNING 
      tty_struct[i].tty_inhibited  =    RUNNING ;
      tty_struct[i].tty_attr       =    0x700 ;//黑底
    }
    //  设置中断函数 底层入口  
    set_vec(0x21,(u32_t*)tty_int,0,0);			//设置键盘中断
    enable_int(1);			                //开启键盘中断
}

i32_t  do_char_int ()
{
	u8_t  m ;
	i32_t  replyee , caller ;
	struct tty_line  *tp ;
    #ifdef _KERNEL_DEBUG
	DispStr( " char_int " );
    #endif
	u8_t   *buf_ptr0 , *buf_ptr1  ;
        u8_t   ch ,  n , tty , count  ;
	setlock() ;
	buf_ptr0   = tty_mess.CHAR_ADDRESS ;
	//  从硬件的缓冲拷贝到缓冲保护区 以防止竞争状态  
	count         = buf_ptr0[0]  ;
	n             = count + count  ; 
	buf_ptr1      = tty_copy_buf ;
	buf_ptr0      +=  2 ;   //这里1个字节储存当前字符个数，1个储存大小
	while ( n-- )
	{
           *buf_ptr1++  = *buf_ptr0++ ;
	}
	// 将硬件的缓冲 字符个数置0 
	buf_ptr0   = tty_mess.CHAR_ADDRESS ;
	*buf_ptr0  = 0 ;  //这个开始没有放在lock与unlock之间
	restore() ;
	buf_ptr1  = tty_copy_buf  ;
	// 处理积累下的字符
	while  ( count--  > 0 )
	{
		tty = *buf_ptr1++ ;
		ch  = *buf_ptr1++ ;
		in_char(tty , ch );
		//此处留下接口 处理进程相关的i/o读写
		tp = &tty_struct[tty];
		if ( tp->tty_inleft > 0  )
		{
			m = tp->tty_mode & RAW  ;
			// 已经收集一些行 或者为原始模式
			if ( tp->tty_lfct > 0 || m == 1 && tp->tty_incount > 0  )
			{
				m   = rd_chars(tp);
				replyee = tp->tty_incaller ;
				caller  = tp->tty_inproc   ;
                if ( m > 0 )
                {
                   m = NOT_SUSPENDED ;
                   tp->tty_incount  = 0 ;
                }
				tty_reply(REVIVE,replyee,caller,m,0,0);
			}
		}
	}	
	return 0;
}

void in_char(  u8_t tty , u8_t ch )
{
    register  struct tty_line  *tp ; 
	tp = &tty_struct[tty];
	ch = make_break(ch);   //通过键盘映射 若为功能符号则返回0不必回显
	if ( 0 == ch || 0xE0 == ch  )
	{
		return ;
	}
	*tp->tty_intail++ = ch ;
	if ( tp->tty_intail == &tp->tty_inqueue[TTY_IN_BYTES] )
	{
		tp->tty_intail = &tp->tty_inqueue[0];
	}
	//  输入已满一行
	if ( ch == '\n' || ch == MARKER )
	{
		tp->tty_lfct++ ;
	}
	tp->tty_incount++ ;
	// 在显示屏上回显该字符
	echo(tp , ch);
}

void  echo( register struct tty_line  *tp, u8_t ch  )
{
  if ( ch != MARKER )
        out_char( tp , ch );
  flush( tp );
}

i32_t sprintf( u8_t **dest , u8_t *src , u8_t len  )
{
  u8_t *pdest ,*psrc, i ;
  pdest = *dest ;
  psrc = src ;
  for ( i = 0 ; i < len && *psrc != 0 ; i++ ) 
	  *pdest++ = *psrc++ ;
  *pdest = 0 ;
  *dest  = pdest ;
  return 0 ;
}

void write ( struct tty_line *tp , u8_t *buf )
{
  u8_t *psrc ;
  psrc = buf ;
  for ( ; *psrc != 0 ; psrc++ )
  {
	  echo( tp , *psrc ) ;
  }
}
//开始由于这个缓冲放在printk里面 导致将栈使用完
//结果老是不能正确运行
static  u8_t  buf[1024] , tmp[18] ;

i32_t printk( u8_t *fmt , ... )
{
	u8_t *psrc , *pdest ,*p;
	i32_t  a , len ;
	struct tty_line *tp = &tty_struct[0] ;
	tp->tty_attr = 0x700 ;
	va_list ap ;
	va_start(ap,fmt) ;
	psrc  = fmt ;
	for ( pdest = buf   ; *psrc != 0  ;  )
	{
        if ( *psrc != '%' ) 
	    {
		    *pdest++ = *psrc++ ;
		    continue ;
	    }
	    psrc++;
        switch ( *psrc++ )
	    {
		    case 'd':
			    {
                     a =  va_arg(ap,i32_t) ;
                     Memset( tmp , 18 );
                     tmp[0] = '0' ;
                     tmp[1] = 'x' ;
                     itoa( a , tmp+2 );
                     sprintf( &pdest, tmp , sizeof ( tmp )  );
                     break ; 
			    }
		     case 's':
			     {
                     p    = va_arg( ap , u8_t *) ; 
                     len  = strlen(p ) ;
                     sprintf( &pdest,p,len );	 
                     break ;
			     }
		    default :
			     return 1 ;// 错误发生
	    }
	}
	*pdest = 0 ;
	setlock();
	write( tp , buf );
	restore();
	va_end ( ap );
	return  0 ;
}


void out_char ( struct tty_line *tp , u8_t ch )
{
     //移动光标
     switch ( ch )
     {
	     case   ENTER    :        //这里的坐标即设备坐标
		     {
		     	move_to ( tp , 0  , ++tp->tty_row ) ;
		     	return ;
		     }
	     case   TAB      :
		     {
			 echo( tp , ' ' );  //  仅仅显示4个空格
		     echo( tp , ' ' );
			 echo( tp , ' ' );
			 echo( tp , ' ' );
		     return ;
		     }
	     case   BACKSPACE:
		     {
			     chuck( tp );  //从输入队列移除一个字符
		         move_to( tp , --tp->tty_column , tp->tty_row ) ;
		         echo( tp , ' ' );
		         move_to( tp , --tp->tty_column , tp->tty_row ) ;
			     return  ;
		     }
	     case  UP:
		     {
			     DispStr( " up " );
			     move_to ( tp , tp->tty_column , --tp->tty_row ) ;
			     return ;
		     }
	     case DOWN:
		     {
			     DispStr( " down " ) ;
			     move_to ( tp , tp->tty_column , ++tp->tty_row ) ;
			     return ;
		     }
	     case LEFT:
		     {
			     DispStr ( " left " ) ;
			     move_to ( tp , --tp->tty_column , tp->tty_row ) ;
			     return ;
		     }
	     case RIGHT:
		     {
			     DispStr ( "right " ) ;
			     move_to ( tp ,++tp->tty_column , tp->tty_row ) ;
			     return ;
		     }
         case F1:
             {
                 print_proc();
                 return ;
             }
	     default  :
             {
                 if ( tp->tty_column >= LINE_WIDTH )
                 {
                     return ;
                 }
                 if ( tp->tty_outwords == TTY_RAM_WORDS )
                 {
                     flush( tp ); 
                 }
                 tp->tty_outqueue[tp->tty_outwords++] = tp->tty_attr | ch ;
                 tp->tty_column++ ;
                 if ( tp->tty_column >= LINE_WIDTH )
                 {
                     move_to ( tp , 0 , ++tp->tty_row ) ; 
                 }
                 return ;
             }
     }

}

void chuck( struct tty_line *tp )
{
  if (0 == tp->tty_incount )
	  return ;
  if ( tp->tty_incount >=  2 )
  {
     tp->tty_incount  -= 2 ;
  }
  if ( (tp->tty_intail - 2 ) >=  &tp->tty_inqueue[0] )
  {
      tp->tty_intail  -=  2 ; 
  }
  else
  {
	  tp->tty_intail = &tp->tty_inqueue[TTY_IN_BYTES - 1 ];
  }
}

void move_to ( struct tty_line *tp , i32_t  x , i32_t  y )
{
  if ( x < 0 || x >= LINE_WIDTH || y < 0 || y > SCR_LINES )
        return ;
  tp->tty_column  = x ;
  tp->tty_row     = y ;
  //移动光标这里省略
  tp->tty_vid  =  y*LINE_WIDTH + y*LINE_WIDTH + x + x ;  //避免使用乘法
}

void flush( struct tty_line *tp )
{
   if ( tp->tty_outwords == 0 )
   {
      	return ;
   }
   vid_copy(tp->tty_outqueue ,tp->tty_org , tp->tty_vid , tp->tty_outwords);
   tp->tty_vid  += 2*tp->tty_outwords ;
   if ( tp->tty_vid >= 4000 ) 
   {
       tp->tty_vid    = 0 ;
	   tp->tty_column = 0 ;  // 这里是权益之计
	   tp->tty_row    = 0 ;
   }
   tp->tty_outwords  = 0 ;
   //  更新tty及硬件的相应的值
}

u8_t make_break( u8_t  ch )
{
    u8_t  make   , code , c , shift , is_true  ;
    c      = ch & 0x7F ;            //通过断码来对应字符 因为最高位表示键是按下还是弹起
    if ( 0xE0 == ch )              //  若为功能键， 进入状态1, 这里的判断和状态机一样  
    {
	    e0 = 1 ;
	    return 0 ;
    }
    if (  1 ==  e0  )   //判断是否为功能键
    {
	    // 特殊功能键的make 和 break 也要特殊算  :)   
	    shift = 2 ;
	    is_true = ( ch == 0x48 || ch == 0x4B || ch == 0x50 || ch == 0x4D ) ; // 没有对所有的功能键进行检测  仅仅检测上下左右方向键
	    make  = is_true  ? 1 : 0 ;	     
    }
    else
    {
             shift  = ( l_shift || r_shift ) ? 1 : 0  ;
             make   = ch & 0x80  ? 0 : 1;   //检测是通码还是断码
    }
    code   = keymap[ c*MAP_COLS + shift];
    e0     = 0;
    if ( code < 0x80  )    //128个普通字符
    {
        //  处理普通键的释放
        if ( 0 == make )
        {
             code = 0 ;
        }
        return code ;
    }
    //  处理特殊字符 这里仅仅处理 ctrl shift alt建
    //  这里的值和keyboard.h里有关
    switch ( code - 0x80 )
    {
	case  1:
		  l_ctrl = make  ;
		  break ;
	case  2:
		  r_ctrl = make  ;
		  break ;
	case  3:
		  l_shift = make ;
		  break ;
	case  4:
		  r_shift = make ;
		  break ;
	case  5:
		  l_alt   = make ;
		  break ;
	case  6:
		  r_alt   = make ;
		  break ;
	case  17 :
	case  18 :
	case  19 :
	case  20 :
    case  21 :
		  {
			//  	处理比较特殊的方向键
			if ( 0 == make )
			{
				code  = 0 ;
			}
		  	return code ;  //处理上下方向键
		  }
	default  :
		  break ;
    }
    return 0 ;
}
message keyboard_mess ; 

void	keyboard_handler()
{
	i32_t  k ;
	u8_t scan_code , tty ;
        scan_code   =   port_in(0x60)    ;
	k           =   scan_code & 0x80 ;
	//  若 k > 0  则有健释放 并且释放的键不是alt , ctrl , shift 
	if ( k > 0 && scan_code != 0xE0 )
	{
		// 释放的键不是方向键   
		if (  scan_code!=0xAA && scan_code!=0x9D&&scan_code!= 0xB8&&
		scan_code!=0xC8&&scan_code!=0xCB&&scan_code!=0xD0&&scan_code!=0xCD  )
		{
		  return ; 
		}
	}
	tty         =   current_tty  ;
    #ifdef _KERNEL_DEBUG
	DispInt( scan_code );
    #endif
        if ( ( k = driver_buf[0] ) < driver_buf[1] )   //缓冲没有满
	{
	     driver_buf[0]++ ;
	     k  =  k + k     ;
	     k  +=  2  ;
	     driver_buf[ k ]      =  tty       ;
	     driver_buf[ k + 1]   =  scan_code ;	     
	     //  发送消息到tty任务
	     keyboard_mess.m_type        =  CHAR_INT  ;   
	     keyboard_mess.CHAR_ADDRESS  =  driver_buf;
         interrupt(TTY_TASK , &keyboard_mess  );	
	}
	else
	{
	     port_out(INT_M_00 , ENABLE);
	}
	return;
}


void do_read(struct tty_line *tp ,  message *m_ptr )
{
       i32_t code = 0 , caller ;
       // 已经有人在等了 则不响应这个请求
       if ( tp->tty_inleft > 0  )
       {
	        tty_reply(TASK_REPLY,m_ptr->m_source,m_ptr->REP_PROC_NR,E_TTY_AGAIN,0,0);
	  return ;
       }
       // 拷贝信息到tty_struct
       tp->tty_incaller = m_ptr->m_source     ;
       tp->tty_inproc   = m_ptr->TTY_PROC_NR  ;
       tp->tty_in_laddr = m_ptr->TTY_ADDRESS  ;
       tp->tty_inleft   = m_ptr->TTY_COUNT    ;
       #ifdef _KERNEL_DEBUG
       printf(" tty read:%d,addr:%d  ",tp->tty_inleft,tp->tty_in_laddr );
       #endif
       // 尽量取得数据 或者什么都没有 
       if ( tp->tty_incount > 0 )
       {
           code    = rd_chars(tp);
       }
       caller  = (i32_t)tp->tty_inproc ;
       if ( code == 0 )
       {
           code = SUSPENDED ;
       }
       tty_reply(TASK_REPLY,m_ptr->m_source,caller,code,0,0);
}

void  do_write( struct tty_line *tp , message *m_ptr )
{
      //  拷贝参数到tty structurre  
      tp->tty_outcaller = m_ptr->m_source ;
      tp->tty_outproc   = m_ptr->m_source ;
      tp->tty_out_laddr = m_ptr->TTY_ADDRESS ;
      tp->tty_outleft   = m_ptr->TTY_COUNT   ;
      tp->tty_waiting   = WAITING            ;
      tp->tty_cum       =  0  ;
      //  运行终端驱动
      (*(tp->tty_devstart))(tp);
}

void  do_cancel(struct tty_line *tp ,  message *m_ptr )
{

}

void  do_ioctl(struct tty_line *tp ,  message *m_ptr )
{

}

void tty_reply(i32_t code,i32_t replyee,i32_t proc_nr, i32_t status, i32_t extra,i32_t other )
{
    // 发送一个消息到某个读或写终端的进程 
    // 请注意为什么这里可以将消息缓冲存放在局部的
    // 栈空间 但这样做不会有问题的 因为该进程必须等待
    // 这条消息 否则系统出错了 
    message m ;
    m.m_type      = code    ;
    m.REP_PROC_NR = proc_nr ;
    m.REP_STATUS  = status  ;
    m.TTY_FLAGS   = extra   ;
    m.TTY_SPEK    = other   ;
    send(replyee,&m);
}

i32_t  rd_chars( struct tty_line *tp )
{
    i32_t    in_vir  , ct , cum  , cooked  ; 
    i32_t left , buf_ct  , enough ;
    PROCESS *rp   ;
    u8_t *tty_ptr , ch  ;
    //  首先检测是否有足够的数据 若有则直接拷贝到user空间 若没有
    //  则发送一条消息到fs，挂起用户程序 ，等到有足够多的数据时
    //  直接拷贝到用户空间 并且通知fs一条消息 
    cum     =   0 ;
    ct      =   0 ;
    enough  =   0 ;
    // 这里我仅仅用两种模式 原始模式和杜撰模式
    cooked  =   (tp->tty_mode&RAW)?0:1;
    #ifdef _KERNEL_DEBUG
    DispStr( tp->tty_inhead);
    #endif
    while ( tp->tty_inleft > 0 )
    {
	// 单次传送tty_inleft tty_incount TTY_BUF_SIZE  较小的 
	buf_ct  = MIN(tp->tty_inleft,tp->tty_incount)  ;
	buf_ct  = MIN(buf_ct,TTY_BUF_SIZE)             ;
	tty_ptr = tty_buf                              ;
	while ( buf_ct -- > 0 )
	{
	   ch = *(tp->tty_inhead++);
	   if ( tp->tty_inhead == &tp->tty_inqueue[TTY_IN_BYTES] )
	   {
		tp->tty_inhead  = tp->tty_inqueue ;
	   }
	   *tty_ptr++ = ch ;
	   ct++ ;
	   if ( ch == '\n' || ch == MARKER  )
	   {
		tp->tty_lfct-- ;
		enough++ ;
		if ( cooked )
		{
			break ;
		}
	   }
	}
    // 字符串结尾补0
    *tty_ptr  = 0 ;
    ct++ ;
	//  拷贝数据到user空间
	copy_data(TTY_TASK,tty_buf,tp->tty_inproc,tp->tty_in_laddr,ct);
	tp->tty_in_laddr += ct ;
	cum              += ct ;
	tp->tty_inleft   -= ct ;
	tp->tty_incount  -= ct ;
	if ( tp->tty_incount == 0 || enough )
	{
		break ;
	}
    }
    // 设想shell上读终端字符 一般也就读一行而已 
    tp->tty_inleft =  0  ;
    tp->tty_incount = 0 ;
    return cum ;   
}


void  console( struct  tty_line *tp  )
{
  i32_t  count   ;
  u8_t   c , *out_vir   ;
  // 等下计算向终端写入的地址 
  out_vir   = tp->tty_out_laddr ;
  while ( tp->tty_outleft > 0 && tp->tty_inhibited  == RUNNING    )
  {
	c = get_byte( tp->tty_outproc  , (u8_t*)tp->tty_out_laddr );
	out_char(tp,c);
	tp->tty_out_laddr++ ;
	tp->tty_outleft-- ;
  }
  flush(tp);
  // 更新终端数据结构
  count = tp->tty_out_laddr - out_vir ;
  tp->tty_cum += count ;
  if ( tp->tty_outleft == 0 )
  {
	finish(tp,tp->tty_cum );
  }
}

// 取一个字节 从指定的物理地址
u8_t  get_byte( i32_t caller , u8_t *out_vir )
{
	u8_t  *user_phy ,   ch = 0   ;
	user_phy = umap( caller , (u8_t*)out_vir ) ;
	if ( user_phy != 0  )
	{
		ch  = *user_phy     ;
	}
	return    ch ;
}

void finish(struct tty_line *tp , i32_t code  )
{
  i32_t replyee , caller ;
  // 终端i/o完成
  tp->tty_outwords  = 0 ;
  tp->tty_outleft   = 0 ;
  if ( tp->tty_waiting == NOT_WAITING )
  {
	return ;
  }
  replyee  = (i32_t)tp->tty_outcaller          ;
  caller   = (i32_t)tp->tty_outproc            ;
  tty_reply(TASK_REPLY,replyee,caller,code,0,0);
  tp->tty_waiting  = NOT_WAITING               ;
}

void print_proc()
{
  i32_t i  ;
  PROCESS *rp ;
  i = 0 ;
  //4个字符4个空格
  printf("\nproc   name   m_ptr\n");
  for ( rp=&proc[0] ; rp < &proc[PROCE_NR] ; rp++ )
  {
     if ( rp->p_flag & P_SLOT_FREE  )
     {
       printf("%d    %s    %d\n",rp-&proc[0],rp->p_name,rp->m_ptr);
     }
  }
}
