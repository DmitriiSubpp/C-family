#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//===========================================================================
// основные формулы

// error = actual - expected -> ошибка для выходного нейрона

// weights_delta = error * sigmoid(x) * (1 - sigmoid(x)) -> число, на которое надо корректировать веса

// weight_1 = weight_1 - output1 * weights_delta * learning_rate -> корректировка веса

// error = weight * weights_delta -> ошибка для скрытого нейрона

//===========================================================================

// активационная функция, которая ограничивает значение не нейроне, в данном случае используется сигмойд
float sigmoid(float x){
    x *= -1.0;
    return 1/(1 + exp(x));
}

void train(void){ // тренировка сети

    float to_h1_weight[] = {0.58, 0.2, 0.28}; // веса для первого нейрона скрытого слоя
    float to_h2_weight[] = {0.76, 0.2, 0.45}; // веса для второго нейрона скрытого слоя
    float h1_w = 0.3, h2_w = 0.7;             // веса для выходного нейрона

    float error;                // ошибка нейрона

    float learning_rate = 0.05; // обучающий коэффициент (подбирается имперически)
    int epochs = 5000;          // Количество эпох (подбирается имперически)

    float weight_delta;         // значение, на которое надо корректировать веса

    int cases[8][4] = {         // обучающая выборка, где четвёртый элемент - ожидаемый результат.
                {0,0,0,0},
                {0,0,1,1},
                {0,1,0,0},      // т.е. если на улице дождь и нам не скучно и не зовет друг, то ожидаемый результат - гулять не пойдем.
                {0,1,1,0},
                {1,0,0,1},
                {1,0,1,1},      // т.е. если на улице нет дождя, нам скучно и нас зовет друг, то ожидаемый результат - гулять пойдем.
                {1,1,0,0},
                {1,1,1,1}
        };

    for (int epoch = 0; epoch < epochs; epoch++){        // Проходимся по эпохам. Эпоха - это один цикл обучения.
                                                          // При каждой итерации нейросеть сравнивает получившийся результат с ожидаемым.
                                                          // И на основе этого корректирует веса методом обратного распространения ошибки.


        for (int c = 0; c < 8; c++){                      // В каждой эпохе проходимся по каждому из 8 случаю и корректируем веса, в зависимости от полученной ошибки

                float x = 0.0;

                for(int i = 0; i < 3; i++)                // Умножаем три входных состояния на веса первого скрытого нейрона
                    x += cases[c][i] * to_h1_weight[i];   // Складываем все произведения
                float h1 = sigmoid(x); // hidden neuron 1 // И ищем выходное значение первого скрытого нейрона

                x = 0.0;
                for(int i =0; i < 3; i++)                 // Умножаем три входных состояния на веса второго скрытого нейрона
                    x += cases[c][i] * to_h2_weight[i];   // Складываем все произведения
                float h2 = sigmoid(x); // hidden neuron 2 // И ищем выходное значение второго скрытого нейрона


                float actual = sigmoid(h1*h1_w + h2*h2_w); // Ищем значение, которое получится на выходном нейроне


                error = actual - cases[c][3];   // Ошибка выходного нейрона
                weight_delta = error * sigmoid(h1*h1_w + h2*h2_w) * (1 - sigmoid(h1*h1_w + h2*h2_w));   // Ищем значение, на которое будем корректировать веса (используется производная сигмоидной ф-ции)

                //if (epochs < 100)
                  //printf("epo: %d , wd: %f\n", epochs, h2);

                h1_w = h1_w - h1 * weight_delta * learning_rate; // считаем сдвиг веса первого скрытого нейрона
                h2_w = h2_w - h2 * weight_delta * learning_rate; // считаем сдвиг веса второго скрытого нейрона

                float h1_w_delta = weight_delta; // буфера для хранения weight_delta
                float h2_w_delta = weight_delta;

                error = h1_w * h1_w_delta;              // ошибка первого скрытого нейрона
                weight_delta = error * h1 * (1.0 - h1); // значение для корректировки весов входных нейронов
                for(int i =0; i < 3; i++)
                    to_h1_weight[i] = to_h1_weight[i] - cases[c][i] * weight_delta * learning_rate; // корректировка весов входящих нейронов, соеденённых с первым скрытым нейроном


                error = h2_w * h2_w_delta;              // ошибка первого скрытого нейрона
                weight_delta = error * h2 * (1.0 - h2); // значение для корректировки весов входных нейронов
                for(int i =0; i < 3; i++)
                    to_h2_weight[i] = to_h2_weight[i] - cases[c][i] * weight_delta * learning_rate; // корректировка весов входящих нейронов, соеденённых со вторысм скрытым нейроном

//===========================================================================
                //printf("epo: %d, output: %f, expected: %d\n", epochs, h1*h1_w + h2*h2_w, cases[c][3]);
//===========================================================================
        }
    }
// вывод весов
    for(int i =0; i < 3; i++)
        printf("%f ", to_h1_weight[i]);

    printf("\n");

    for(int i =0; i < 3; i++)
        printf("%f ", to_h2_weight[i]);

    printf("\n%f\n%f", h1_w, h2_w);

}

void predict(float boring, float rain, float friend1){

    float input[] = {boring, rain, friend1};
    float to_h1_weight[] = {-2.57, 3.01, -2.62};
    float to_h2_weight[] = {2.56, -2.98, 2.46};
    float h1_w = -7.6, h2_w = 3.7;


    float x = 0.0;
    for(int i =0; i < 3; i++)
            x += input[i] * to_h1_weight[i];
        float h1 = sigmoid(x); // hidden neuron 1

    x = 0.0;
    for(int i =0; i < 3; i++)
        x += input[i] * to_h2_weight[i];
    float h2 = sigmoid(x); // hidden neuron 2

    if (sigmoid(h1*h1_w + h2*h2_w) >= 0.5) printf("Придётся идти\n");
    else printf("Лучше не пойду\n");

}

int main()
{
    int rain  =  0; // на улице дождь
    int friend1 = 0; // зовет друг
    int boring = 1; // дома скучно

    //train();
    predict(boring, rain, friend1);
    
    return 0;
}
