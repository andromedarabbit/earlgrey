#pragma once
#include "EarlgreyAssert.h"

#define EARLGREY_ENUM_TO_STRING(enumName) TEXT(#enumName)


//////////////////////////////////////////////////////////////////////////
// 아래 매크로는 Game Programming Gems 3에서 가져왔다.
// 다른 매크로 기법은 득보다 실이 많다고 생각한다. 
// 특히 디버깅이 힘들다는 점을 고려해서 도입하지 않는다.

//////////////////////////////////////////////////////////////////////////
//Macro Trick #2: Compile-Time Constants from Binary Representations
// Thanks to Jeff Grills for providing this implementation

/*
//Usage
const int nibble = BINARY1(0101);       // 0x5
const int byte   = BINARY2(1010,0101);  // 0xa5

// 0xa5a5a5a5
const int dword = BINARY8(1010,0101,1010,0101,1010,0101,1010,0101);
*/

//Macro source:

#define HEX_DIGIT_0000 0
#define HEX_DIGIT_0001 1
#define HEX_DIGIT_0010 2
#define HEX_DIGIT_0011 3
#define HEX_DIGIT_0100 4
#define HEX_DIGIT_0101 5
#define HEX_DIGIT_0110 6
#define HEX_DIGIT_0111 7
#define HEX_DIGIT_1000 8
#define HEX_DIGIT_1001 9
#define HEX_DIGIT_1010 a
#define HEX_DIGIT_1011 b
#define HEX_DIGIT_1100 c
#define HEX_DIGIT_1101 d
#define HEX_DIGIT_1110 e
#define HEX_DIGIT_1111 f

#define HEX_DIGIT(a)        HEX_DIGIT_ ## a

#define BINARY1H(a)         (0x ## a)
#define BINARY1I(a)         BINARY1H(a)
#define BINARY1(a)          BINARY1I(HEX_DIGIT(a))

#define BINARY2H(a,b)       (0x ## a ## b)
#define BINARY2I(a,b)       BINARY2H(a,b)
#define BINARY2(a,b)        BINARY2I(HEX_DIGIT(a), HEX_DIGIT(b))

#define BINARY8H(a,b,c,d,e,f,g,h) (0x##a##b##c##d##e##f##g##h)
#define BINARY8I(a,b,c,d,e,f,g,h) BINARY8H(a,b,c,d,e,f,g,h)
#define BINARY8(a,b,c,d,e,f,g,h)  BINARY8I(HEX_DIGIT(a),    \
	HEX_DIGIT(b), HEX_DIGIT(c), HEX_DIGIT(d), HEX_DIGIT(e),    \
	HEX_DIGIT(f), HEX_DIGIT(g), HEX_DIGIT(h))

