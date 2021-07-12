#include <stdio.h>
#define DEBUG 0
#define SIZE 100

/*
Awesome_calc v1.1

Цель: отработка полученных знаний на практике


1) В массив problem записывается форматированное выражение , например: введено (2 + qw5) , а записалось как  (2+5)
   (в end записывается символ, и если он равен 'e', то программа завершается. Символ 'r' остается в  массиве, т.к. он сигнализирует о том, что вместо него необходимо подставить сохраненный результат)

2) Затем в функции operations, которая принимает форматированный массив problem, в переменную x и y записываются количество цифр в первом и втором операнде соответственно.
   Дальше в игру вступают два цикла for, которые переводят набор символов "123" в число 123 для первого операнда и второго.
   стоит заметить, что переменной x соответствует номер элемента в массиве, значение которого - знак операции (problem[x]- знак операции).
   Поэтому все элементы, стоящие до problem[x] образауют первый операнд, после - второй.
   при этом проверятеся условие, что символ до знака операции не равен'r'.Если так, то операнду присваивается сохраненное значение

   перевод происходит примерно так:
   переменная num_class - это разряд числ, изначально num_class = 1, с каждой итерацией чикла for, разряд увеличивается: десятки , сотни, тысячи
   Шаг 1: пусть до problem[x] размещен такой набор символов: '2''3', тогда номер символа '3' в массиве это x-1, а разряд num_class - единицы.
          Выходит, что число 3 = num_class*(problem[x-1] - '0')
   Шаг 2: Номер элемента, стоящего после '3' [х-2] и разряд умножится на 10, т.к. переходим к десяткам (num_class*10)
          Выходит, что число 20 = num_class*10(problem[x-2] - '0')
   Шаг 3: складывая результат первого и второго шагов получаем число 23
   аналогично и для второго операнда

3) Зная чему равен первый и второй операнд и знак операции(problem[x]) производим операцию

дополнительно производится отчистка массива после каждой проведенной операции

 */

void clear_list(char problem[], char end[]) // Функция удаляет из строки пробелы и прочий мусор, приводя строку к виду |операнд|операция|операнд|
{
   int i=0, c=0; // i - счётчик,  с - хранит символ

   while ((c=getchar())!=EOF && c!='\n') {
        end[0]=c;
        if ((int)end[0]=='e') {break;} // выход из функции clear_list
        if (((int)c<42 || (int)c>57) && (int)c != '^' && (int)c != '%' && (int)c != 'r') continue;


        problem[i]=c;
        i++;

   }
}
// возведение числа a в степень b
int degree(int a, int b)
{
    int degree =1;
    for (int i=0; i<b ; i++) {degree*=a;}
    return degree;
}


int operations(char problem[], int saved_result)
{

static int x,y; // x-номер символа операции
int i=0; // счётчик
int operand_x=0; // первый операнд
int num_class; // разряд числа
int operand_y=0; // второй операнд


// считаем кол-во символов в операндах
for (x=0,i=0; (int)problem[i] != '+' && (int)problem[i] != '*' && (int)problem[i] != '^' && (int)problem[i] != '%' && (int)problem[i] != '/' && (int)problem[i] != '-' ; x++, i++) {} //считает количество цифр в первом операнде
for (y=0,i=x+1; (int)problem[i] != '\0'; y++, i++) {}//считает количество цифр во втором операнде


// производим перевод символов в цифры
for(i=x, num_class=1; i>0; i--, num_class*=10)
    {
        if ((int)problem[0]=='r') {operand_x = saved_result; break;}
        else operand_x += num_class*(problem[i-1]-'0');
    }


for(i=x+y, num_class=1; i>x; i--, num_class*=10)
    {
        if ((int)problem[x+1]=='r') {operand_y = saved_result; break;}
        else operand_y += num_class*(problem[i]-'0');
    }


if(DEBUG)printf("\n--> Операнды х, у: %d %d\n",operand_x , operand_y);

// производим операции над операндами
if ((int)problem[x]=='+') printf(" = %d",saved_result=operand_x + operand_y);
else if ((int)problem[x]=='-') printf(" = %d",saved_result=operand_x - operand_y);
else if ((int)problem[x]=='/')
    {
        if (operand_y!=0) {printf(" = %d",saved_result=operand_x / operand_y);}
        else printf("\nНельзя делить на 0\n");

    }
else if ((int)problem[x]=='*') printf(" = %d",saved_result=operand_x * operand_y);
else if ((int)problem[x]=='^') printf(" = %d",saved_result=degree(operand_x, operand_y));
else if ((int)problem[x]=='%')
    {
        if (operand_y!=0) printf(" = %d",saved_result=operand_x % operand_y);
        else printf("\nОшибка\n");
    }
else printf("Неизвестная операция\n");

for (char i = SIZE-1; i>=0; i--) {problem[i]='\0';} // очистка массива с выражением

return saved_result;
}


int main () {

char end[1]; // используется для завершения программы
int saved_result=0; // результат проведенный операции сохраняется здесь
char problem[SIZE]= {}; // массив для хранения выражения


printf("Alpha version: \nРезультат деления - целое число\nПоддерживаются только целые числа\n");
printf("-----------------------------------------------------------------------\n");

printf("\nКак работать с awesome_calc?\n\nВведите ваше выражение (например 2+5) и awesome_calc посчитает результат\n\nРезультат проведенной операции сохраняется, чтобы его использовать введите r вместо числа (например 2+r)\n\n");
printf("Введите exit для завершения программы\n\n");
printf("Поддерживаемые операции: +, -, /, *, ^, %%\n\n");


// главный цикл
while(1){


printf("\n\nr - результат прошлой операции (%d)\nВведите выражение\n: ", saved_result);

clear_list(problem,  end); // форматируем массив сиволов

if ((int)end[0]=='e') {break;} // выход из программы

saved_result = operations(problem, saved_result); // производим операции

if(DEBUG) printf("\n--> сохраненный результат%d \n", saved_result);

printf("\f");
}

return 0;
}
