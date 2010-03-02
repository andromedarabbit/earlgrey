#if defined(WIN64)
// #import "C:\Program Files\Common Files\System\ado\msado15.dll" version("2.8") no_namespace rename("EOF", "EndOfFile")
#import "C:\Program Files\Common Files\System\ado\msado15.dll" version("2.8") rename_namespace("RawADO") rename("EOF", "EndOfFile")
#else
// #import "libid:2A75196C-D9EB-4129-B803-931327F72D5C" version("2.8") named_guids no_namespace rename("EOF", "EndOfFile") // msado15.dll
#import "libid:2A75196C-D9EB-4129-B803-931327F72D5C" version("2.8") named_guids rename_namespace("RawADO") rename("EOF", "EndOfFile") // msado15.dll
#endif

#include <ole2.h>
#include <conio.h>


#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#endif
