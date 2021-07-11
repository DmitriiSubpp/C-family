#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

char help[] = ("\
Использование: ./alg [ПАРАМЕТР]... [АРГУМЕНТ1]...\n\
Печатает результаты алгебраических алгоритмов.\n\
\n\
Если ПАРАМЕТР и аргументы не заданы, читает стандартный ввод\n\
                      и анализирует введеные числа на простоту\n\
\n\
\n\
  -g, --gcd           нод чисел (алгоритм Евклида)\n\
  -l, --lcm           нок чисел (алгоритм Евклида)\n\
  -e, --gcd-extended  линейное представление нод двух чисел\n\
                                (расширенный алгоритм Евклида)\n\
  -f, --factorize     разложить число на простые множители\n\
  -m, --mod           модулярная арифметика - деление по модулю\n\
  -u, --euler         фунция Эйлера от числа\n\
\
      --help     показать эту справку и выйти\n\
      --version  показать информацию о версии и выйти\n\
\n\
\n\
Примеры:\n\
  ./alg -g 1443 480 выводит :       1443 = 480  *  3 + 3\n\
                                    480  = 3    * 160 + 0\n\
                                    НОД (a, b) = 3\n\
\n\
  ./alg -l 1443 480 выводит :       1443 = 480  *  3 + 3\n\
                                    480  = 3    * 160 + 0\n\
                                    НОД (a, b) = 3\n\
                                    НОК [1443, 480] = 230880\n\
\n\
  ./alg -e 1443 480 выводит :       НОД (a, b) = 3\n\
                                    : 3 = 1 * 1443-3 * 480\n\
\
");

char version[] = "alg 0.1\nАвтор программы - Dmitrii Subbp\n"; 

int getGCD(int a, int b){
    static int r;
    r = a % b;
    printf("%-4d = %-4d * %2d + %d\n", a, b, (a-r)/b, r);
    if (r != 0) getGCD(b, r);
    else 
    {
        printf("НОД (a, b) = %d\n", b);
        return b;
        
    }
}

void getLCM(int a, int b){
    printf("НОК [%d, %d] = %d\n",a, b, a*b / getGCD(a,b));
}


int a, b, n =0;
void getGCDExtended(int a1, int b1){
    static int r, q[10], u[10], v[10];
    u[0] = 1; u[1] = 0;
    v[0] = 0; v[1] = 1;

    r = a1 % b1;

    if (n>=2) u[n] = u[n-2] - u[n-1]*q[n-2];
    if (n>=2) v[n] = v[n-2] - v[n-1]*q[n-2];

    q[n] = (a1 - r) / b1;

    n++;

    if (r != 0) getGCDExtended(b1, r);
    else {
        u[n] = u[n-2] - u[n-1]*q[n-2];
        v[n] = v[n-2] - v[n-1]*q[n-2];

        printf("НОД (a, b) = %d\n", b1);
        if (u[n] < 0) printf(": %d = (%d) * %d%+d * %d\n", b1, u[n], a, v[n], b);
        else printf(": %d = %d * %d%+d * %d\n", b1, u[n], a, v[n], b);
        n = 0 ;
    }

}

int main(int argc, char *argv[]){
    
    int option;
    int option_index;

    char optstring[] = "gle";
    
    int g = 0, l = 0, e = 0;
    

    const struct option long_options[] = {
		{"gcd",          optional_argument,NULL,'g'},
		{"lcm",          optional_argument,NULL,'l'},
		{"gcd-extended", optional_argument,NULL,'e'},
		{"help",         optional_argument,NULL,'h'},
        {"version",      optional_argument,NULL,'v'},
        {NULL,0,NULL,0}

	};

        
    while((option = getopt_long(argc, argv, optstring, long_options, &option_index)) != -1)
    {
        switch(option)
        {
            case 'g':
                g = 1;
                break;

            case 'l':
                l = 1;
                break;

            case 'e':
                e = 1;
                break;
            
            case 'h':
                printf("%s",help);
                break;
                
            case 'v':
                printf("%s",version);
                break;

            default:
                printf("По команде «--help» можно получить дополнительную информацию.\n");
                exit(1);
                break;
        }
        option_index = -1;
    }
    
    int options[] = {g, l, e};
    int nums[5] = {0};
    
     for (int i = 1, n = 0; i < argc; i++)
        if (argv[i][0] != '-')
        {
            a = atoi(argv[i]);
            if (a < 0) a = 0 - a;
            nums[n++] = a;
        }

    a = nums[0]; b = nums[1];
    if (g && !l && a != 0 && b != 0) getGCD(a,b);
    if ((l || (l && g)) && a != 0 && b != 0) getLCM(a, b);
    if (e && b != 0) getGCDExtended(a, b);
}
