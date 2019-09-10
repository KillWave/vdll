
#flag -I ./include/dll.h
#flag -L ./lib
#flag -ldll
#flag -lffi
#flag -ldl
#include "include/dll.h"
//加载动态库并且返回句柄
fn open(path string) int {
    //C.RTLD_LAZY
    handle := C.dll_open(path.str)
    if handle == -1 {
        println('error: open dll failed!')
        return -1
    }
    return 0 
}
// //关闭动态库
fn dl_close(){
    C.dll_close()
}
fn main(){
    open('./libtest.so')
    fliename := 'm_pr'
    C.dll_call(fliename.str,3,4,5,6)
    dl_close()
}