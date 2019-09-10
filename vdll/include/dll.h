#ifndef Dll_H
#define Dll_H
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <ffi.h>
void (*fnc)(void);
void* handle;
int dll_open(const char* path);
void* dll_call(const char* fn_name,int len,...);
void dll_close(void);
//void*(*fuc)(void* arg1,void* arg2,void* arg3,void* arg4,void* arg5,void* arg6,void* arg7,void* arg8,void* arg9,void* arg10,void* arg11,void* arg12);
#endif