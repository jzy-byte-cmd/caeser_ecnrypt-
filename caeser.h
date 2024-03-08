int fit_size(char *str, char *f_str, int str_length); 
// 将目标字符串调整为刚好容纳所有字符（包括'\0'）的大小  
//返回 字符串的新大小
void check_realloc(char *p);                          // 检查realloc是否成功


char *read_file(char *file_name);        //读取文件   返回 文件内所有内容的字符串指针

int acquire_bias();    //获取偏移量  返回  int 偏移量

char *caeser_encrypt(char *str,int bias);        //加密字符串  返回加密后的字符串指针

void write_result(char *str);               //写入结果

int to_int(char *str);       //将字符串转化为int

#ifndef SCANF_FAILED
    #define SCANF_FAILED -6
#endif

#ifndef FILE_OPEN_FAILED
    #define FILE_OPEN_FAILED -1
#endif

#ifndef FCLOSE_FAILED
    #define FCLOSE_FAILED -4
#endif


/*
exit 列表    
            -1 文件打开失败 open_failed
            -2 malloc_failed
            -3 realloc_failed
            -4 fclose_failed
            -5 system()_failed 函数错误
            -6 scanf_failed  输入错误
*/