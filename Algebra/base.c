#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
 * Орешкин Дмитрий Евгеньевич 
 * ККСО-03-20 
 * Задание 5
 * 
 */

int toBase(long int num, char *base_s){
    // функция для перевода из десятичной сс в любую другую
    char nums[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    int base = atoi(base_s) , i = 0;            // base - конечная сс, i - счётчик

    int num_in_base[1024];                      // тут хранится число в конечной системе счисления по одной цифре 123 -> {1,2,3}

    for (num; num >= base; num /= base)         //делим десятичное число на конечную сс и остаток записываем в массив num_in_base
        num_in_base[i++] = num % base;

    num_in_base[i] = num % base;                //дозапись остатка в массив num_in_base

    for (i; i>=0; i--)
        printf ("%c", nums[num_in_base[i]]);            
}
    
    
long int fromBase(char *num, char *base){
    // функция для перевода числа из исходной сс в десятичную
    char nums[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    long int result = 0 , i = 0; // result - переменная в которой лежит десятичное число, i - счётчик

    int j = strlen(num) - 1;     // степень для возведения
    int num_len = strlen(num);   

    while(i < num_len)           
        result += (strchr(nums, num[i++]) - nums) * pow(atoi(base), j--);
    
    printf("%ld", result);
    return result;
}


int main(int argc, char** argv)
{
    
    //toBase(atoi(argv[0]), argv[1]);
    toBase(4513985418, "47");
    putc('\n', stdout);
    fromBase("JW2Wji", "47");
    putc('\n', stdout);
    return 0;
}

