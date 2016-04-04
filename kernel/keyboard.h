#ifndef  _KEYBOARD_H_
#define  _KEYBOARD_H_

#define NR_SCAN       88
#define  MAP_COLS     3

#define   MARKER        0 

#define   ESC         27
#define   BACKSPACE   15
#define   TAB         13
#define   ENTER       10
#define   L_CTRL      129
#define   R_CTRL      130
#define   L_SHIFT     131
#define   R_SHIFT     132
#define   L_ALT       133
#define   R_ALT       134
#define   CAPS_LOCK   135
#define   NUM_LOCK    136
#define   SCROLL_LOCK 137
#define   PAD_DOT     138   

#define  INSERT       139
#define  DELETE       140
#define  HOME         141
#define  END          142
#define  PAGEUP       143
#define  PAGEDOWN     144
#define  UP           145
#define  DOWN         146
#define  LEFT         147
#define  RIGHT        148

#define  F1           149
#define  F2           150
#define  F3           151
#define  F4           152
#define  F5           153
#define  F6           154
#define  F7           155
#define  F8           156
#define  F9           157
#define  F10          158
#define  F11          159
#define  F12          160

/* scan-code      !shift      shift    E0 XX */
u8_t  keymap[NR_SCAN*MAP_COLS]={  
			                /*0x0 -none*/	0,	0   ,	0,
			                /*0x1-esc */	ESC,	ESC ,	0,
			               /*0x2 -1*/      '1',     '!' ,   0,
			                               '2',     '@' ,   0,
			                               '3',     '#',    0,
			                               '4',     '$',    0,
			                               '5',     '%',    0,
			                               '6',     '^',    0,
			                               '7',     '&',    0,
			                               '8',     '*',    0,
			               /*0xa  -9*/     '9',     '(',    0,
			                               '0',	')',    0,
			                               '-',     '_',    0,
			                               '=',     '+',    0,
		                                       BACKSPACE,BACKSPACE,0,
		                                       TAB,     TAB,    0,
		                                       'q',     'Q',    0,
		                                       'w',     'W',    0,
		                                       'e',     'E',    0,
   	                                               'r',     'R',    0,
                                                       't',     'T',    0,
                                                       'y',     'Y',    0,
                                                       'u',     'U',    0,
                                                       'i',     'I',    0,
                                                       'o',     'O',    0,
                                                       'p',     'P',    0,
                                                       '[',     '{',    0,
                                                       ']',     '}',    0,
                                                        ENTER,  ENTER,  0,
                                     /*L  Ctrl*/        L_CTRL,L_CTRL, R_CTRL,
                                                       'a',     'A',    0,
                                                       's',     'S',    0,
                                                       'd',     'D',    0,
                                                       'f',     'F',    0,
                                                       'g',     'G',    0,
                                                       'h',     'H',    0,
                                                       'j',     'J',    0,
                                                       'k',     'K',    0,
                                                       'l',     'L',    0,
                                                       ';',     ':',    0,
                                                       '\'',    '"',    0,
                                                       '`',     '~',    0,
                                                       L_SHIFT, L_SHIFT,0,
                                                       '\\',    '|',    0,
                                                       'z',     'Z',    0,
                                                       'x',     'X',    0,
                                                       'c',     'C',    0,
                                                       'v',     'V',    0,
                                                       'b',     'B',    0,
                                                       'n',     'N',    0,
                                                       'm',     'M',    0,
                                                       ',',     '<',    0,
                                                       '.',     '>',    0,
                                                       '/',     '?',    0,
                                                       R_SHIFT,R_SHIFT, 0,
                                                       '*',     '*',    0,
                                                       L_ALT,L_ALT, R_ALT,
                                                       ' ',     ' ',    0,
                                                       CAPS_LOCK,0,     0,
                                                       F1,      F1,     0,
                                                       F2,      F2,     0,
                                                       F3,      F3,     0,
                                                       F4,      F4,     0,
                                                       F5,      F5,     0,
                                                       F6,      F6,     0,
                                                       F7,      F7,     0,
                                                       F8,      F8,     0,
                                                       F9,      F9,     0,
                                                       F10,     F10,    0,
                                       /*0x45*/        NUM_LOCK,NUM_LOCK,0,
                                       /*0x46*/        SCROLL_LOCK,SCROLL_LOCK,0,
                                                       '7',     '7',    HOME,
                                                       '8',     '8',    UP,
                                                       '9',     '9',    PAGEUP,
                                                       '-',     '-',    0,
                                                       '4',     '4',    LEFT,
                                                       '5',     '5',    0,
                                                       '6',     '6',    RIGHT,
                                                       '+',     '6',    0,
                                                       '1',     '1',    END,
                                                       '2',     '2',    DOWN,
                                                       '3',     '3',    PAGEDOWN,
                                                       '0',     '0',    INSERT,
                                                       '.',     '.',    DELETE,
                                                       0,       0,      0,
                                                       0,       0,      0,
                                                       F11,     F11,    0,
                                                       F12,     F12,    0
};

#endif
