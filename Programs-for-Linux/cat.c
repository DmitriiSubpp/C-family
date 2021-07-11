#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>

#define no_argument            0
#define required_argument      1
#define optional_argument      2

/*

Реализовано:

1. Все флаги
2. перенаправление потока ввода в поток вывода
3. Возможность использовать несколько опций подряд

1. -s -b -n выполняются всегда первыми, где у -s высший приоритет, а у -b приоритет выше чем у -n
2. Все остальные опции выполняются если нет опций из п.1 или после них
3. Оставшиеся опции выполняются в таком порядке приоритета: -A -E -T -v -e -t

*/


char help[] = ("\
Использование: ./cat [ПАРАМЕТР]… [ФАЙЛ]…\n\
Печатает слияние ФАЙЛ(ов) на стандартный вывод.\n\
\n\
Если ФАЙЛ не задан или задан как -, читает стандартный ввод.\n\
\n\
  -A, --show-all           синоним -vET\n\
  -b, --number-nonblank    нумеровать непустые строки при выводе\n\
  -e                       подавляет -n, синоним -vE\n\
  -E, --show-ends          показывать $ в конце каждой строки\n\
  -n, --number             нумеровать все строки при выводе\n\
  -s, --squeeze-blank      выводить не более одной пустой строки при повторе\n\
  -t                       синоним -vT\n\
  -T, --show-tabs          показывать символы табуляции как ^I\n\
  -u                       (игнорируется)\n\
  -v, --show-nonprinting   использовать запись с ^ и M-, за исключением\n\
                           символов перевода строки и табуляции\n\
      --help     показать эту справку и выйти\n\
\n\
\n\
Примеры:\n\
  ./cat f - g  Выводит содержимое f, потом стандартный ввод, потом g.\n\
  ./cat        Копирует стандартный ввод на стандартный вывод\n\
");

char version[] = "user's cat 1.2\nАвтор программы - Dmitrii Oreshkin\n"; 

int getOptions(char *str, int *options){

    for (int i = 0; i < strlen(str); i++)
    {
        if (options[0] || options[3])        // опции -A -E
        {

            if (str[i] == '\a' ||
                str[i] == '\b' ||
                str[i] == '\n' ||
                str[i] == '\v' ||
                str[i] == '\f' ||
                str[i] == '\t' ||
                str[i] == '\0' ||
                str[i] == '\r')
            {

                if (str[i] == '\t' && !options[3])          // опция -T
                {
                    printf("^I");
                    continue;
                }
                if (str[i] == '\a' && !options[3])         
                    printf("^G");
                if (str[i] == '\b' && !options[3])        
                    printf("^H");
                if (str[i] == '\n' && !options[3])       
                    printf("^J");
                if (str[i] == '\v' && !options[3])        
                    printf("^K");
                if (str[i] == '\f' && !options[3])        
                    printf("^L");
                if (str[i] == '\0' && !options[3])        
                    printf("$");
                if (str[i] == '\r' && !options[3])          
                    printf("^M");
                
                if (options[3] && str[i] == '\n')
                    printf("$");

            }
            printf("%c", str[i]);
        }


        else if (!options[0] && options[7]) // опция -T (Если вызвана только она)
        {

            if (str[i] == '\t')
            {
                printf("^I");
                continue;
            }

            printf("%c", str[i]);
        }

        else if (!options[0] && (options[9] || (options[2] && options[6]))) // опция -v
        {

            if (str[i] == '\a' ||
                str[i] == '\b' ||
                str[i] == '\v' ||
                str[i] == '\f' ||
                str[i] == '\r')
            {

                if (options[7] && str[i] == '\t')          // опция -T
                    printf("^I");
                
                if (str[i] == '\a' && !options[3])         
                    printf("^G");
                if (str[i] == '\b' && !options[3])        
                    printf("^H");
                if (str[i] == '\v' && !options[3])        
                    printf("^K");
                if (str[i] == '\f' && !options[3])        
                    printf("^L");
                if (str[i] == '\0' && !options[3])        
                    printf("$");
                if (str[i] == '\r' && !options[3])          
                    printf("^M");

                //printf("$");
            }

            printf("%c", str[i]);
        }

        else if (!options[0] && !options[6] && options[2]) // опция -e
        {

            if (str[i] == '\a' ||
                str[i] == '\0' ||
                str[i] == '\b' ||
                str[i] == '\n' ||
                str[i] == '\v' ||
                str[i] == '\f' ||
                str[i] == '\r')
            {
                
                if (str[i] == '\a' && !options[3])         
                    printf("^G");
                if (str[i] == '\b' && !options[3])        
                    printf("^H");
                if (str[i] == '\v' && !options[3])        
                    printf("^K");
                if (str[i] == '\f' && !options[3])        
                    printf("^L");
                if (str[i] == '\0' && !options[3])        
                    printf("$");
                if (str[i] == '\r' && !options[3])          
                    printf("^M");
                
                printf("$");
            }
            printf("%c", str[i]);
        }

        else if (!options[0] && !options[2] && options[6]) // опция -t
        {

            if (str[i] == '\a' ||
                str[i] == '\b' ||
                str[i] == '\v' ||
                str[i] == '\f' ||
                str[i] == '\t' ||
                str[i] == '\r')
            {
                if (options[7] && str[i] == '\t')
                {// опция -T
                    printf("^I");
                    continue;
                }
                
                if (str[i] == '\a' && !options[3])         
                    printf("^G");
                if (str[i] == '\b' && !options[3])        
                    printf("^H");
                if (str[i] == '\v' && !options[3])        
                    printf("^K");
                if (str[i] == '\f' && !options[3])        
                    printf("^L");
                if (str[i] == '\0' && !options[3])        
                    printf("$");
                if (str[i] == '\r' && !options[3])          
                    printf("^M");
                
                //printf("$");
            }

            printf("%c", str[i]);
        }
    }
    return 0;
}

int i = 1; // счетчик строк для -n -b
int n = 0; // счетчик пустых строк для -s

void stdin_to_stdout(int *options){
    /* перенаправление stdin в stdout с возможностью использования некоторых опций */
    char str[4096];

    while(!feof (stdin))
        if(fgets(str, 4096, stdin))
        {
            if (options[5] &&  strlen(str) == 1)
                n++;

            if (options[5] && n > 1 && strlen(str) == 1)
                continue;

            if (options[5] && strlen(str) != 1)
                n = 0;
                
            if ((options[1] || options[4]))
            {
                printf("%7d  ",i++);
                if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                    getOptions(str, options);
                else
                    printf("%s", str);
            }
            else
                if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                    getOptions(str, options);
                else
                    printf("%s", str);
        }
}


int get_s_b_n_Options(char *str, int *options){

            /* Если есть флаг -s, то при помощи трёх условий, написанных ниже,
               программа будет пропускать повторы пустых строк */
                if (options[5] &&  strlen(str) == 1)
                    n++;

                if (options[5] && n > 1 && strlen(str) == 1)
                    return 0;

                if (options[5] && strlen(str) != 1)
                    n = 0;

                if (options[1] || options[4] || options[5])
                /* Если в опциях есть -s -b -n, то выполняется этот блок. Дальше проверяются другие опции*/
                {
                    if (options[5] && !options[1] && !options[4]) // проверка опции -s
                    {
                        if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                            getOptions(str, options);
                        else
                            printf("%s", str);
                    }

                    if (options[1] && strlen(str) == 1)           // опция -b (проверка пустой строки)
                    {
                        if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                            getOptions(str, options);
                        else
                            printf("%s", str);

                    }

                    else if (options[1] && strlen(str) != 1)      // опция -b (проверка не пустой строки)
                    {
                        printf("%7d  ",i++);
                        if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                            getOptions(str, options);
                        else
                            printf("%s", str);
                    }

                    if (options[4] && !options[1])                // проверка опции -n
                    {
                        printf("%7d  ",i++);
                        if (options[0] || options[2] || options[3] || options[6] || options[7] || options[8] || options[9])
                            getOptions(str, options);
                        else
                            printf("%s", str);

                    }
                }
                else                                              // Если не заданы флаги -s -b -n
                    getOptions(str, options);

                return 0;
}


int getFile(char *name, int *options){

    FILE* file = fopen(name, "r");

    if (file == NULL)
    {
        printf("cat: %s: Нет такого файла или каталога\n", name);
        return 0;
    }


//===========
// Простой вывод без опций
    int c, n = 0;
    for (int j = 0; j < 10; j++)
        if (options[j])
            n++;
    if (n == 0)
        while((c = fgetc(file)) != EOF)
            printf("%c", c);
    else
        n = 0;

//===========
// Вывод с опциями
    char *str = (char*) malloc(1024 * sizeof(char*));
    int j = 0, delta = 0;
    c = 0;

    while ((c=fgetc(file)) != EOF)
    {
        if (strlen(str)>1024+delta)
        {
            delta += 1024;
            str = (char*) realloc(str, (1024+delta) * sizeof(char*));
        }
            str[j++] = c;


        if (c == '\n')
        {
            get_s_b_n_Options(str, options);          // Берем строку из файла, при необходимости динамически увеличиваем выделенную память
            memset(str,0,1024+delta);
            j = 0;
            delta = 0;
        }
    }

    free(str);
    fclose(file);

    return 0;
}

int main(int argc, char *argv[]){

    int option;
    int option_index;

    char optstring[] = "AbeEnstTuv";

    const struct option long_options[] = {
		{"show-all",        optional_argument,NULL,'A'},
		{"number-nonblank", optional_argument,NULL,'b'},
		{"show-ends",       optional_argument,NULL,'E'},
		{"number",          optional_argument,NULL,'n'},
		{"squeeze-blank",   optional_argument,NULL,'s'},
		{"show-tabs",       optional_argument,NULL,'T'},
		{"show-nonprinting",optional_argument,NULL,'v'},
		{"help",            optional_argument,NULL,'h'},
        {"version",         optional_argument,NULL,'u'},
		{NULL,0,NULL,0}
		//{"help",required_argument,NULL,'h'},
		//{"version",required_argument,NULL,'f'},

	};

    int A = 0, b = 0, e = 0, E = 0, n = 0, s = 0, t = 0, T = 0, u = 0, v = 0;

    while((option = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        switch(option)
        {
            case 'A':
                A = 1;
                break;

                case 'b':
                    b = 1;
                    break;

                    case 'e':
                        e = 1;
                        break;

                        case 'E':
                            E = 1;
                            break;

                            case 'n':
                                n = 1;
                                break;

                                case 's':
                                    s = 1;
                                    break;

                            case 't':
                                t = 1;
                                break;

                        case 'T':
                            T = 1;
                            break;

                    case 'u':
                        printf("%s", version);
                        break;

                case 'v':
                    v = 1;
                    break;

            case 'h':
                printf("%s", help);
                break;

            default:
                printf("По команде «cat --help» можно получить дополнительную информацию.\n");
                break;
        }
        option_index = -1;
    }

    int options[] = {A, b, e, E, n, s, t, T, u, v};

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] != '-')
            getFile(argv[i], options);   // вывод с опциями
        else if (!strcmp(argv[i], "-"))
            stdin_to_stdout(options);    // stdin в stdout с опциями
    }

    if (argc == 1)
        stdin_to_stdout(options);        // stdin в stdout без опций

    return 0;
}
