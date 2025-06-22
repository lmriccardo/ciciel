#pragma once

#include <term.h>

namespace ccl::cli::ui
{

// All possible ASCII basic printable characters
#define CCL_KEY_SPACE       0x20
#define CCL_KEY_EXC_MARK    0x21
#define CCL_KEY_DQUOTES     0x22
#define CCL_KEY_NUM_SIGN    0x23
#define CCL_KEY_DOLLAR      0x24
#define CCL_KEY_PERCENT     0x25
#define CCL_KEY_AMPERSAND   0x26
#define CCL_KEY_SQUOTE      0x27
#define CCL_KEY_O_BRACKET   0x28
#define CCL_KEY_C_BRACKET   0x29
#define CCL_KEY_ASTERISK    0x2a
#define CCL_KEY_PLUS        0x2b
#define CCL_KEY_COMMA       0x2c
#define CCL_KEY_H_MINUS     0x2d
#define CCL_KEY_DOT         0x2e
#define CCL_KEY_SLASH       0x2f
#define CCL_KEY_0           0x30
#define CCL_KEY_1           0x31
#define CCL_KEY_2           0x32
#define CCL_KEY_3           0x33
#define CCL_KEY_4           0x34
#define CCL_KEY_5           0x35
#define CCL_KEY_6           0x36
#define CCL_KEY_7           0x37
#define CCL_KEY_8           0x38
#define CCL_KEY_9           0x39
#define CCL_KEY_COLON       0x3a
#define CCL_KEY_SEMI_COLON  0x3b
#define CCL_KEY_LT          0x3c
#define CCL_KEY_EQ          0x3d
#define CCL_KEY_GT          0x3e
#define CCL_KEY_QST_MARK    0x3f
#define CCL_KEY_AT_SIGN     0x40
#define CCL_KEY_SHIFT_A     0x41
#define CCL_KEY_SHIFT_B     0x42
#define CCL_KEY_SHIFT_C     0x43
#define CCL_KEY_SHIFT_D     0x44
#define CCL_KEY_SHIFT_E     0x45
#define CCL_KEY_SHIFT_F     0x46
#define CCL_KEY_SHIFT_G     0x47
#define CCL_KEY_SHIFT_H     0x48
#define CCL_KEY_SHIFT_I     0x49
#define CCL_KEY_SHIFT_J     0x4a
#define CCL_KEY_SHIFT_K     0x4b
#define CCL_KEY_SHIFT_L     0x4c
#define CCL_KEY_SHIFT_M     0x4d
#define CCL_KEY_SHIFT_N     0x4e
#define CCL_KEY_SHIFT_O     0x4f
#define CCL_KEY_SHIFT_P     0x50
#define CCL_KEY_SHIFT_Q     0x51
#define CCL_KEY_SHIFT_R     0x52
#define CCL_KEY_SHIFT_S     0x53
#define CCL_KEY_SHIFT_T     0x54
#define CCL_KEY_SHIFT_U     0x55
#define CCL_KEY_SHIFT_V     0x56
#define CCL_KEY_SHIFT_W     0x57
#define CCL_KEY_SHIFT_X     0x58
#define CCL_KEY_SHIFT_Y     0x59
#define CCL_KEY_SHIFT_Z     0x5a
#define CCL_KEY_O_S_BRACKET 0x5b
#define CCL_KEY_BACK_SLASH  0x5c
#define CCL_KEY_C_S_BRACKET 0x5d
#define CCL_KEY_CARET       0x5e
#define CCL_KEY_UNDERSCORE  0x5f
#define CCL_KEY_BACK_THICK  0x60
#define CCL_KEY_A           0x61
#define CCL_KEY_B           0x62
#define CCL_KEY_C           0x63
#define CCL_KEY_D           0x64
#define CCL_KEY_E           0x65
#define CCL_KEY_F           0x66
#define CCL_KEY_G           0x67
#define CCL_KEY_H           0x68
#define CCL_KEY_I           0x69
#define CCL_KEY_J           0x6a
#define CCL_KEY_K           0x6b
#define CCL_KEY_L           0x6c
#define CCL_KEY_M           0x6d
#define CCL_KEY_N           0x6e
#define CCL_KEY_O           0x6f
#define CCL_KEY_P           0x70
#define CCL_KEY_Q           0x71
#define CCL_KEY_R           0x72
#define CCL_KEY_S           0x73
#define CCL_KEY_T           0x74
#define CCL_KEY_U           0x75
#define CCL_KEY_V           0x76
#define CCL_KEY_W           0x77
#define CCL_KEY_X           0x78
#define CCL_KEY_Y           0x79
#define CCL_KEY_Z           0x7a
#define CCL_KEY_O_BRACE     0x7b
#define CCL_KEY_V_BAR       0x7c
#define CCL_KEY_C_BRACE     0x7d
#define CCL_KEY_TILDE       0x7e

// CTRL+<char> is defined as ( SHIFT + <char> ) & 0x1F
#define CCL_CTRL_MODIFIER 0x1F
#define CCL_KEY_CTRL_A    0x01
#define CCL_KEY_CTRL_B    0x02
#define CCL_KEY_CTRL_C    0x03
#define CCL_KEY_CTRL_D    0x04
#define CCL_KEY_CTRL_E    0x05
#define CCL_KEY_CTRL_F    0x06
#define CCL_KEY_CTRL_G    0x07
#define CCL_KEY_CTRL_H    0x08
#define CCL_KEY_CTRL_I    0x09
#define CCL_KEY_CTRL_J    0x0a
#define CCL_KEY_CTRL_K    0x0b
#define CCL_KEY_CTRL_L    0x0c
#define CCL_KEY_CTRL_M    0x0d
#define CCL_KEY_CTRL_N    0x0e
#define CCL_KEY_CTRL_O    0x0f
#define CCL_KEY_CTRL_P    0x10
#define CCL_KEY_CTRL_Q    0x11
#define CCL_KEY_CTRL_R    0x12
#define CCL_KEY_CTRL_S    0x13
#define CCL_KEY_CTRL_T    0x14
#define CCL_KEY_CTRL_U    0x15
#define CCL_KEY_CTRL_V    0x16
#define CCL_KEY_CTRL_W    0x17
#define CCL_KEY_CTRL_X    0x18
#define CCL_KEY_CTRL_Y    0x19
#define CCL_KEY_CTRL_Z    0x1a

#define CCL_KEY_ESCAPE 0x1B // Generally identify the escape char

// Other Control characters
#define CCL_KEY_ENTER     0x0A
#define CCL_KEY_BACKSPACE 0x7F

// This would work if setupterm is previously called
#define TERMCAP( cap ) tigetstr( cap )

#define CCL_ESCAPE_KEY_UP      TERMCAP("kcuu1")
#define CCL_ESCAPE_KEY_DOWN    TERMCAP("kcud1")
#define CCL_ESCAPE_KEY_LEFT    TERMCAP("kcub1")
#define CCL_ESCAPE_KEY_RIGHT   TERMCAP("kcuf1")
#define CCL_ESCAPE_KEY_HOME    TERMCAP("khome")
#define CCL_ESCAPE_KEY_END     TERMCAP("kend" )
#define CCL_ESCAPE_KEY_INSERT  TERMCAP("kich1")
#define CCL_ESCAPE_KEY_DELETE  TERMCAP("kdch1")
#define CCL_ESCAPE_KEY_PG_UP   TERMCAP("kpp"  )
#define CCL_ESCAPE_KEY_PG_DOWN TERMCAP("knp"  )
#define CCL_ESCAPE_MOUSE_ENTER TERMCAP("kmous")

#define CCL_KEY_UP    0x01
#define CCL_KEY_DOWN  0x02
#define CCL_KEY_RIGHT 0x03
#define CCL_KEY_LEFT  0x04

inline const char* CCL_ESCAPE_KEY_FN( int kf_num )
{
    char cap[8]; snprintf( cap, sizeof(cap), "kf%d", kf_num );
    return TERMCAP( cap );
}

#define CCL_KEY_FN( x ) x
#define CCL_KEY_FN1  CCL_KEY_FN(  1 )
#define CCL_KEY_FN2  CCL_KEY_FN(  2 )
#define CCL_KEY_FN3  CCL_KEY_FN(  3 )
#define CCL_KEY_FN4  CCL_KEY_FN(  4 )
#define CCL_KEY_FN5  CCL_KEY_FN(  5 )
#define CCL_KEY_FN6  CCL_KEY_FN(  6 )
#define CCL_KEY_FN7  CCL_KEY_FN(  7 )
#define CCL_KEY_FN8  CCL_KEY_FN(  8 )
#define CCL_KEY_FN9  CCL_KEY_FN(  9 )
#define CCL_KEY_FN10 CCL_KEY_FN( 10 )
#define CCL_KEY_FN11 CCL_KEY_FN( 11 )
#define CCL_KEY_FN12 CCL_KEY_FN( 12 )

#define CCL_KEY_HOME    CCL_KEY_SHIFT_H
#define CCL_KEY_END     CCL_KEY_SHIFT_F
#define CCL_KEY_INSERT  CCL_KEY_2
#define CCL_KEY_DELETE  CCL_KEY_3
#define CCL_KEY_PG_UP   CCL_KEY_5
#define CCL_KEY_PG_DOWN CCL_KEY_6

}