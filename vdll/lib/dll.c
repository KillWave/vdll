/* test.c */
#include "../include/dll.h"
int dll_open(const char* path){  
    handle = dlopen(path, RTLD_NOW|RTLD_GLOBAL);
    if (!handle) {
        printf("dlopen error:%s\n", dlerror());
        return -1;
    }
    return 0;
}
void* dll_call(const char* fn_name,int len,...){
    fnc = dlsym(handle, fn_name);
    va_list args;
    va_start(args,len);
    ffi_cif cif;    // 函数调用所需的上下文 
    ffi_type **arg_types = alloca(sizeof(ffi_type *) *len);    // 参数类型指针数组
    void **arg_values = alloca(sizeof(void *)*len);    // 参数值指针数组
    int i;
    // 根据被调用函数的参数类型进行设定.
    for ( i = 0; i < len; i++)
    {
        // 设置函数参数.
        int temp = va_arg(args,unsigned long int);
        arg_types[i] = &ffi_type_sint32;
        void *ffiArgPtr = alloca(arg_types[i]->size);
        int *argPtr = ffiArgPtr;
        *argPtr = temp;
        arg_values[i] = ffiArgPtr;
    }
    
    ffi_type *returnFfiType = &ffi_type_sint32;
    ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int)len, returnFfiType, arg_types);
    // 这里 ffi_prep_cif 的第三个参数为被调用函数参数数量, 第四个参数为返回值类型的指针.
   if (ffiPrepStatus == FFI_OK) {
        //生成用于保存返回值的内存
        void *returnPtr = NULL;
        if (returnFfiType->size) {
            returnPtr = alloca(returnFfiType->size);
        }
        //根据cif函数原型，函数指针，返回值内存指针，函数参数数据调用这个函数
        ffi_call(&cif, fnc, returnPtr, arg_values);
        //拿到返回值
        return returnPtr;
    }
}
void dll_close(void){
    dlclose(handle);
}