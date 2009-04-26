#pragma once

/**
* \ref http://support.microsoft.com/kb/155196
* \example #pragma message(__LOC__"Need to do 3D collision testing")
*/
#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "