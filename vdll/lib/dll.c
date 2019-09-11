/* test.c */
#include "../include/dll.h"
static void *handle;
int dll_open(const char *path)
{
    handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (!handle)
    {
        printf("dlopen error:%s\n", dlerror());
        return -1;
    }
    return 0;
}
void arguments(char str_format, int i, int flag)
{
    //printf("%c.%d\n",str_format,i);
    switch (str_format)
    {
    case 'i':
        // 设置函数参数.
        if (flag)
        {
            int temp = va_arg(dll.args, int);
            dll.arg_types[i] = &ffi_type_sint;
            void *ffiArgPtr = malloc(dll.arg_types[i]->size);
            int *argPtr = ffiArgPtr;
            *argPtr = temp;
            dll.arg_values[i] = ffiArgPtr;
        }
        else
        {
            //printf("%c\n",str_format);
            dll.returnFfiType = &ffi_type_sint;
            /* code */
        }

        break;
    case 'f':
        if (flag)
        {
            double temp = va_arg(dll.args, double);
            dll.arg_types[i] = &ffi_type_double;
            void *ffiArgPtr = malloc(dll.arg_types[i]->size);
            double *argPtr = ffiArgPtr;
            *argPtr = temp;
            dll.arg_values[i] = ffiArgPtr;
        }
        else
        {
            dll.returnFfiType = &ffi_type_double;
            /* code */
        }
        break;
    // case 'f':
    //     if (flag)
    //     {
    //         float temp = va_arg(dll.args, float);
    //         dll.arg_types[i] = &ffi_type_float;
    //         void *ffiArgPtr = malloc(dll.arg_types[i]->size);
    //         float *argPtr = ffiArgPtr;
    //         *argPtr = temp;
    //         dll.arg_values[i] = ffiArgPtr;
    //     }
    //     else
    //     {
    //         dll.returnFfiType = &ffi_type_float;
    //         /* code */
    //     }
    //     break;
    case 'c':
        if (flag)
        {
            const char *temp = va_arg(dll.args, char *);
            dll.arg_types[i] = &ffi_type_schar;
            void *ffiArgPtr = malloc(dll.arg_types[i]->size);
            char *argPtr = ffiArgPtr;
            *argPtr=temp;
            dll.arg_values[i] = ffiArgPtr;
        }
        else
        {
            dll.returnFfiType = &ffi_type_schar;
            /* code */
        }
        break;
    case 's':
        if (flag)
        {
            const char *temp = va_arg(dll.args, char *);
            dll.arg_types[i] = &ffi_type_pointer;
            void *ffiArgPtr = malloc(dll.arg_types[i]->size);   
            char **argPtr = ffiArgPtr;
            *argPtr = malloc(sizeof(char)*strlen(temp));
            strcpy(*argPtr, temp);
            dll.arg_values[i] = ffiArgPtr;
        }
        else
        {
            dll.returnFfiType = &ffi_type_pointer;
            /* code */
        }
        break;
    case 'l':
        if (flag)
        {
            long temp = va_arg(dll.args, long);
            dll.arg_types[i] = &ffi_type_slong;
            void *ffiArgPtr = malloc(dll.arg_types[i]->size);
            long *argPtr = ffiArgPtr;
            *argPtr = temp;
            dll.arg_values[i] = ffiArgPtr;
        }
        else
        {
            dll.returnFfiType = &ffi_type_slong;
            /* code */
        }
        break;
    default:
        break;
    }
}

//看可变参处理
void *dll_call(const char *fn_name, int len, const char *str_format, ...)
{
    fnc fn = dlsym(handle, fn_name);
    va_start(dll.args, str_format);
    ffi_cif cif;                                      // 函数调用所需的上下文
    dll.arg_types = alloca(sizeof(ffi_type *) * len); // 参数类型指针数组
    dll.arg_values = alloca(sizeof(void *) * len);    // 参数值指针数组
    int i = 0;
    char ar, res;
    //     // 根据被调用函数的参数类型进行设定.
    while (*str_format)
    {
        switch (*str_format)
        {
        case '%':
            i++;
            break;
        case '/':
            //printf("%c\n",*str_format);
            ar = *str_format++;
            res = str_format[strlen(str_format) - 1];
            //printf("%c\n",res);
            arguments(res, i - 1, 0);
            break;
        default:
            arguments(*str_format, i - 1, 1);
            break;
        }
        str_format++;
    }
    ffi_status ffiPrepStatus = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, (unsigned int)len, dll.returnFfiType, dll.arg_types);
    // // 这里 ffi_prep_cif 的第三个参数为被调用函数参数数量, 第四个参数为返回值类型的指针.

    if (ffiPrepStatus == FFI_OK)
    {
        //生成用于保存返回值的内存
        void *returnPtr = NULL;
        if (dll.returnFfiType->size)
        {
            returnPtr = alloca(dll.returnFfiType->size);
        }
        //根据cif函数原型，函数指针，返回值内存指针，函数参数数据调用这个函数
        ffi_call(&cif, fn, returnPtr, dll.arg_values);
        //释放实参内存
        int n;
        for (n = 0; n < i; n++)
        {
            //free(dll.arg_types[n]);
            free(dll.arg_values[n]); /* code */
        }
        va_end(dll.args);
        switch (res)
        {
        case 'i':
            /* code */
            return *(int *)returnPtr;
            break;
        case 'f':
            /* code */
            return (double *)returnPtr;
            break;
        case 'c':
            /* code */
            return *(char *)returnPtr;
            break;
        case 'l':
            /* code */
            return *(long *)returnPtr;
            break;

        default:
            return returnPtr;
            break;
        }
    }
}
void dll_close(void)
{
    dlclose(handle);
}