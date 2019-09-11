#ifndef Dll_H
#define Dll_H
#include <dlfcn.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ffi.h>
#include <string.h>
typedef void (*fnc)(void);
int dll_open(const char* path);
void *dll_call(const char* fn_name,int len,const char* str_format,...);
void dll_close(void);
struct 
{
    va_list args;
    ffi_type **arg_types;
    void **arg_values;
    ffi_type *returnFfiType;
    ffi_closure *closure;
    /* data */
}dll;
void arguments(char str_format,int i,int flag);
#endif