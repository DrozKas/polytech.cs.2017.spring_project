/**
 * @file paprg.h
 * @brief Заголовочный файл для библиотеки
 */

#ifndef PAPRG_H
#define PAPRG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "portaudio.h"

#define SAMPLE_RATE   (44100)     ///< Частота дискретизации (сколько раз в секунду будет изменяться сигнал на выходное устройство)
#define FRAMES_PER_BUFFER  (64)   ///< Буфер вывода.

#ifndef M_PI
#define M_PI  (3.14159265) ///< Константа Pi
#endif /* M_PI */

/**
 * @brief Структура для передачи данных в функцию обратного вызова.
 */
typedef struct
{
        float *value_sin; ///< Предподсчитанные значение синуса.
        int phase;        ///< Текущее значение синуса, которое должно быть использовано.
        int count_point;  ///< Размер массива value_sin.
} PaData;

/**
 * @brief Функция, которая играет звук заданной частоты заданное время.
 *
 * @param frequency - частота
 * @param mlsec - Время (в милисекундах).
 * @param outputParameters - Информация о устройстве вывода.
 * @returns[int] код ошибки
 */
int PaPrgPlay(
        float frequency,
        unsigned int mlsec,
        PaStreamParameters *outputParameters
);

#endif /* PAPRG_H */
