#include <stdio.h>

#define MAX_STRING_LEN (20)

/**
 * 判断是否是字符
*/
int is_char(char c)
{
    return (c <= 'z' && c >= 'a') ||
            (c <= 'Z' && c >= 'A');
}

/**
 * 字符大小写转换
 * params: flag = 1 大写 转 小写
 *              = 0 小写 转 大写
*/
// unsigned char up_low_trans(int flag, unsigned char c)
// {
//     if(flag && )
// }

/**
 * param flag 大写还是小写 1：大写；0 小写
*/
unsigned char trans_char(int flag, unsigned char c)
{
    //printf("trans_char: %c \n", c);
    if(flag && c > 'Z') return 'A' + (c - 'Z') - 1;  
    else if(!flag && c > 'z') return 'a' + (c - 'z') - 1;

    return c;
}

int main()
{
    unsigned char secret[MAX_STRING_LEN] = "ab";
    unsigned char src_string[MAX_STRING_LEN] = "Bncryptor";
    unsigned char result[MAX_STRING_LEN] = {0};

    int str_len = 9;
    int secret_len = 1;

    unsigned char m, n = 0;
    for(int i = 0; i < str_len; i++)
    {
        m = secret[i % secret_len];

        if(m >= 'A' && m <= 'Z')
        {
            n = m + 32;    //转换成小写
        }
        else
        {
            n = m;
        }

        //printf("%c %c \n", m, n);
        if(src_string[i] <= 'Z' && src_string[i] >= 'A')
        { 
            result[i] = trans_char(1, src_string[i] + (n - 'a'));
        } 
        else if(src_string[i] <= 'z' && src_string[i] >= 'a')
        {
            result[i] = trans_char(0, src_string[i] + (n - 'a'));
            
        }
        else
        {
            result[i] = src_string[i];
        }
        //printf("%d %c %c \n", i, src_string[i], result[i]);
        //printf("%c %c %d %c\n", m, src_string[i], i, result[i]);
    }

    for(int i = 0; i < str_len; i++)
    {
        printf("%c", result[i]);
    }
    printf("\n");

    return 0;
}