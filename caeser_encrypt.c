#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include "caeser.h"


// iconv.h库 编译时需 -l iconv   (小写L)来链接库



/*
exit 列表    详情见caeser.h 头文件
            -1 文件打开失败 open_failed
            -2 malloc_failed
            -3 realloc_failed
            -4 fclose_failed
            -5 system()_failed 函数错误
            -6 scanf_failed  输入错误
*/


int main(int argc,char *argv[])
{
    char *str=NULL;      //要加密的字符串
    char *result=NULL;
    char file_name[255]="";     //文件名  最长不超过255
    int bias=0; //偏移量
    setlocale(LC_ALL,"zh_CN.utf8");
    if(system("chcp 65001")!=0) // 设置cmd 显示编码 utf8
    {
        printf("system() failed.");
        exit(-5);
    }
    if (argc==1)    //不带参数
    {
    bias=acquire_bias();    //获取偏移量
    printf("\n当前目录下的所有文件:\n");
    if(system("dir /a:-d /b"))     //简要显示显示当前目录下所有文件(不包含目录)
    {
        printf("system() failed.");
        exit(-5);
    }

    printf("\n\n请输入需要 加密/解密 的文件名.(同目录下),以回车结束.[暂不支持二进制文件加密]\n");
    int scanf_result=scanf("%s",file_name);
    if(scanf_result==EOF||scanf_result==0)
    {
        printf("\n输入错误.按任意键退出程序.");
        getch();
        exit(SCANF_FAILED);
    }
    }
    else
    {
        if(argc!=5)
        {
            printf("参数错误!\n");
            exit(0);
        }
        else
        {   
            char **f_argv=argv;  //储存第一个参数的地址
            argv++;//指向第2个参数
        
            if(strcmp(*argv,"-b")==0) //输入偏置值
            {
                argv++;    //后移一个参数
                if(sizeof(**argv)>=31)       //int数据溢出
                {
                    printf("参数错误!\n");
                    exit(0);
                }
                bias=to_int(*argv);      //转换为整数
                argv++;
                if(strcmp(*argv,"-i")==0)     //文件名
                {
                    argv++;
                    strcpy(file_name,*argv);
                }
                else
                {
                    printf("参数错误!");
                    exit(0);
                }
            }
            else
            {
                printf("参数错误!");
                exit(0);
            }

            free(f_argv);
            f_argv=NULL;
            argv=NULL;
        }
    }
    str=read_file(file_name);            //读取文件

    result=caeser_encrypt(str,bias);     //加密

    write_result(result);       //写入加密结果
    
    free(str); // 因为 f_str和 str此时 指向同一位置.因此只需释放一次
    // free(str);    重复释放会导致 SIGTRAP
    str=NULL;    //防止野指针

    return 0;
}


int to_int(char *str)
{
    int value=0;
    char *f_str=str;    //记录字符串首元素地址
    str+=strlen(f_str)-1; //将指针移到末尾  '\0'前一个元素
    for(int i=0;str!=f_str-1;i++,str--)
    {
        if(((*str<48)&&(*str!=45))||*str>57)        //不是数字也不是-  负号
        {
            printf("参数错误!");
            exit(0);
        }
        if(*str!='-')
        {
        value+=((int)(ceil(pow(10,i)))*(((int)*str)-48));  //按权重转换
        }
        else
        {
            if(*str=='-')
            {
                if(str!=f_str)  //不是首元素   表明负号在中间
                {
                    printf("参数错误!");
                    exit(0);
                }
                else
                {
                    value*=-1;  //负号
                }
            }
        }

    }
    return value;
}


int fit_size(char *str, char *f_str, int str_length)
{
    for (int i = 0; i < str_length; i++, str++)
    {
        if (*str == '\0') // 遇到'\0'
        {
            str_length = i + 1;                                        // 确定最终的字符串长度 (包括'\0')
            f_str = (char *)realloc(f_str, str_length * sizeof(char)); // realloc 要填首元素指针
            if (f_str != NULL)
            {
                //printf("realloc succeed!\n str_size now is: %d bytes,length is: %d\n", str_length, str_length);
            }
            else
            {
                printf("realloc failed.");
                exit(-3);
            }
        }
    }
    str = f_str; // 复位指针
    return str_length;
}

void check_realloc(char *p)
{
    if (p == NULL)
    {
        printf("realloc failed.");
        exit(-3);
    }

    return;
}

char *read_file(char *file_name)
{
    int str_length = 20;        //字符串长度
    char *str = (char *)malloc(str_length * sizeof(char)); // 分配40字符的空间   一个wchar 占2bytes
    char *f_str = str;
    if (str == NULL) // 分配失败
    {
        printf("malloc failed!");
        exit(-2);
    }
    FILE *fp = fopen(file_name, "r+"); // 打开目标文件
    if (fp == NULL)                       // 打开失败
    {
        printf("file open failed!");
        exit(-1);
    }
    for (int count = 0; !feof(fp); str++, count++)
    {
        *str = fgetwc(fp);
        // printf("%c,%p,%d  ",*str,str,*i);
        if (count >= 19)
        {
            int nums = str - f_str; // nums为两个指针之间的元素个数
            str_length += 20;       // 增加20个字符的长度
            f_str = (char *)realloc(f_str, str_length * sizeof(char));
            check_realloc(f_str);
            // check_realloc(f_str);       //检查是否分配成功

            str = f_str + nums;
            count = 0;
        }
    }
    str--;
    *str = '\0'; // 将最后一个字符改为'\0'
    str = f_str; // 将指针移回字符串首元素

    str_length = fit_size(str, f_str, str_length);

    //printf("\n\n%s", f_str);

    if (fclose(fp) == -1)
    {
        printf("fclose failed.");
        exit(-4);
    }
    free(fp);       //释放指针
    
    return f_str;
}

int acquire_bias()
{
    int bias=0;     //偏移量
    printf("请输入凯撒加密 (Caeser encryption)的加密/解密偏移量(整数).\n示例:A->C 偏移量为2 加密\nC->A 偏移量-2  解密\n请输入[以回车结束]:");
    int scanf_result=scanf("%d",&bias);
    if(scanf_result==EOF||scanf_result==0)      //读取错误
    {
        printf("\n输入错误.按任意键退出程序.\n");
        
        getch();
        exit(SCANF_FAILED);
    }
    return bias;
}


char *caeser_encrypt(char *str,int bias)       //根据偏移量加密字符串
{
    char *f_str=str;
    while(*str!='\0')   //直到字符串尾
    {   
        (*str)=(char)((int)(*str)+bias);
        str++;
    }
    return f_str;
}


void write_result(char *str)
{
    FILE *fp;
    fp=fopen("result.txt","w");
    if(fp==NULL)
    {
        printf("file open failed!");
        exit(FILE_OPEN_FAILED);
    }
    while(*str!='\0')
    {
        fputc(*str,fp);
        str++;
    }
    if(fclose(fp)==-1)
    {
        printf("fclose failed.");
        exit(FCLOSE_FAILED);
    }
    free(fp);

    return;
}


