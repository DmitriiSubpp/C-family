#include <iostream>
#include <cmath>

#define STEP_X 0.1
#define STEP_Y 0.1
#define INTERVAL_X 5.0

using namespace std;

/*
 * Программа сначала заполняет массив с значениями y[], а затем построчно рисует параболу.
 * Точка выводится, если в текущей итерации цикла значение в массиве у[] совпало с текущем значением на оси Оу.
 */

int main(){

    float y[100]; // тут лежат все значения функции y=sin(x)

    for (float x = 0 - INTERVAL_X, i = 0; x < INTERVAL_X; x += STEP_X, i++) //заполнение массива y значениями sin(x) в диапазоне [-INTERVAL_X,INTERVAL_X]
        y[(int)i] = round(sin(x) * 10)/10;           // округляем полученный sin(x) до первого знака после запятой

    for (float Oy = 1.1; Oy >= -1.1; Oy -= STEP_Y) { // идем по оси Oy с шагом STEP и с каждым шагом делаем перебор массива значений функции y

            if (round(Oy * 10)/10 == 0) {            // печать оси Ох на кооринатах y = 0
                for (int i = 0; i < 100; i++)
                    cout << "-";
                cout << "\n";
                continue;
            }

            for (int i = 0; i < 100; i++){                  // цикл, рисующий синусойду
                if (i == 100/2) {cout << "|"; continue;}    // печать оси Оу
                if (y[i] == round(Oy * 10)/10) cout << "*"; // если в массиве встречается такое y[i] которое равно Oy, то печатаем *
                else cout << " ";
                }
            cout << "\n";
    }
    return 0;
}
