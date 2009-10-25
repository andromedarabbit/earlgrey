#pragma once

/**
/* \ref http://support.microsoft.com/kb/155196
 * \ref http://www.viper.pe.kr/cgi-bin/moin.cgi/C%2B%2B_%EC%9C%A0%EC%9A%A9%ED%95%9C_%EB%A7%A4%ED%81%AC%EB%A1%9C
 * \example 
 #pragma EARLGREY_TODO( TODO 메시지 1...! )
 #pragma EARLGREY_FIXME( FIXME 메시지 2...! )
 * 
 */
#define EARLGREY_QUOTE(x)				#x
#define EARLGREY_QUOTE1(x)				EARLGREY_QUOTE(x)
#define EARLGREY_FILE_LINE				__FILE__ "(" EARLGREY_QUOTE1(__LINE__) "): "

#define EARLGREY_TODO( x )				message( EARLGREY_FILE_LINE"[TODO]: " #x "\n" )
#define EARLGREY_FIXME( x )             message( EARLGREY_FILE_LINE"[FIXME]: " #x "\n" )



