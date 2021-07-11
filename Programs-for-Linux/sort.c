#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define no_argument            0
#define required_argument      1
#define optional_argument      2

//#define DEBUG


/*

Реализовано:
1) опции -r и -f;
2) сортировка из stdin с возможностью использования опций из п.1
3) сортировка нескольких файлов подряд
*/


char help[] = ("\n\
Использование: sort [ПАРАМЕТР]… [ФАЙЛ]…\n\
\n\
Печатает сортированное слияние всех ФАЙЛ(ов) на стандартный вывод.\n\
\n\
Если ФАЙЛ не задан или задан как -, читает стандартный ввод.\n\
\n\
Аргументы, обязательные для длинных параметров, обязательны и для коротких.\n\
Параметры, задающие порядок:\n\
\n\
  -b, --ignore-leading-blanks игнорировать начальные пропуски (в разработке)\n\
  -d, --dictionary-order      рассматривать только пропуски, буквы и цифры (в разработке)\n\
  -f, --ignore-case           игнорировать регистр букв\n\
  -r, --reverse               изменить результаты сравнения на противоположные\n\
");


int getStrCount(char *name){

    FILE* file = fopen(name, "r");

    if (file == NULL)
    {
        printf("cat: %s: Нет такого файла или каталога\n", name);
        return 0;
    }

    int str_counter = 0, c = 0;
    while ((c = fgetc(file)) != EOF)
        if (c == '\n')
            str_counter++;

    fclose(file);

    #ifdef DEBUG
        printf("Количество строк: %d\n", str_counter);
    #endif // DEBUG

    return str_counter;
}

void getSort(char **str, int *options, int max){

    char arr_en[63];

    if (!options[0])
        strcpy(arr_en, "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    else
        strcpy(arr_en, "ZYXWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba9876543210");

    if (options[1]) // -f
    {
        for (int i = 0; i < 62; i++)
            for (int n = 0; n < max; n++)
            {
                if ((int)str[n][0]>=62 && str[n][0]<=90)
                {
                    if (str[n][0] + 32 == arr_en[i])
                        printf("%s", str[n]);
                }
                else
                    if (str[n][0] == arr_en[i])
                        printf("%s", str[n]);
            }
    }
    else // вывод без опций, стандартная сортировка
    {
        for (int i = 0; i < 62; i++)
            for (int n = 0; n < max; n++)
                if (str[n][0] == arr_en[i])
                    printf("%s", str[n]);
    }


    //exit(1);
}

void stdin_to_stdout(int *options){
    /* перенаправление stdin в stdout с возможностью использования некоторых опций */
    int max = 1024;

    char **str = (char**) malloc(max * sizeof(char*));
    for(int i = 0; i < max; i++)
        str[i] = (char *)malloc(1024*sizeof(int));

    int j = 0, delta = 0, n = 0;
    int c = 0;

    while ((c=fgetc(stdin)) != EOF)
    // Берем строку из stdin, при необходимости динамически увеличиваем выделенную память
    {
        if (n == max)
            break;

        if (strlen(str[n]) > 1024+delta)
        {
            delta += 1024;
            str = (char**) realloc(str, (1024+delta) * sizeof(char*));
        }
        if (c != ' ' || c != '\t')
            str[n][j++] = c;

        if (c == '\n')
        {
            j = 0;
            delta = 0;
            n++;
        }
    }

    getSort(str, options, max);


}

int getFile(char *name, int *options){

    FILE* file = fopen(name, "r");

    if (file == NULL)
    {
        printf("cat: %s: Нет такого файла или каталога\n", name);
        return 0;
    }

    int max = 0;
    max = getStrCount(name);
    //if (max == 0){printf("Пустой файл\n"); exit(1);}

    char **str = (char**) malloc(max * sizeof(char*));
    for(int i = 0; i < max; i++)
        str[i] = (char *)malloc(1024*sizeof(int));

    int j = 0, delta = 0, n = 0;
    int c = 0;

    while ((c=fgetc(file)) != EOF)
    // Берем строку из файла, при необходимости динамически увеличиваем выделенную память
    {
        if (strlen(str[n]) > 1024+delta)
        {
            delta += 1024;
            str = (char**) realloc(str, (1024+delta) * sizeof(char*));
        }

        if (c != ' ' || c != '\t')
            str[n][j++] = c;

        if (c == '\n')
        {
            j = 0;
            delta = 0;
            n++;
        }
    }

    getSort(str, options, max);

    for(int i = 0; i < max; i++)
        free(str[i]);

    fclose(file);

    return 0;
}

int main(int argc, char *argv[]){

    int option;
    int option_index;

    char optstring[] = "rf";

    const struct option long_options[] = {
		{"ignore-case", optional_argument,NULL,'f'},
		{"reverse",     optional_argument,NULL,'r'},
		{"help",        optional_argument,NULL,'h'},
		{NULL,0,NULL,0}
		//{"help",required_argument,NULL,'h'},
		//{"version",required_argument,NULL,'f'},
	};

    int r = 0, f = 0;

    while((option = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        switch(option)
        {

            case 'r':
                r = 1;
                break;

            case 'f':
                f = 1;
                break;

            case 'h':
                printf("%s", help);
                exit(1);
                break;

            default:
                printf("По команде «cat --help» можно получить дополнительную информацию.\n");
                exit(1);
                break;
        }
        option_index = -1;
    }

    int options[] = {r, f}, flag = 0;

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-'){
            getFile(argv[i], options);
            flag = 1;
        }
        else if (!strcmp(argv[i], "-"))
            stdin_to_stdout(options);    // вывод stdin в stdout заданный как '-'
    }

    if (!flag)
        stdin_to_stdout(options); // вывод stdin в stdout не заданный как '-'

    return 0;
}
