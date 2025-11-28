#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>	// for the Print(...) macro
#include <dlfcn.h>	// for the GET_SYM(...) macro

#define	lenof(_str)			(sizeof(_str) - 1)

#define	cntof(_ary)			(sizeof(_ary) / sizeof((_ary)[0]))

#define	Print(_fmt, ...)	do{printf(_fmt "\r\n", __VA_ARGS__);}while(0);

#define GET_SYM(_ptr, _nam, _lib, _out, _err)		do{	if((_ptr) == NULL)										\
														{														\
															(_ptr) = (typeof(_ptr))dlsym(_lib, _nam);			\
															if((_ptr) == NULL){(_out) = (_err); return (_out);}	\
														}														\
													}while(0)

#endif // UTIL_H
