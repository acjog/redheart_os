;声明外部函数
extern main
extern exit
[SECTION .text]
ALIGN  32
[BITS 32]
global  _start
_start:
;跳转到main入口点 但是不会跳回所以不必call
; main( int argc , char *argv[] )
      call      main
      push      eax
      call      exit
;永远不会执行到此
      jmp	$
