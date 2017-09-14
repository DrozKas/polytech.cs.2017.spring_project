/**
 * @file paprg.c
 * @brief Исходный код библиотеки
 */

#include "paprg.h"

/**
 * @brief Функция обратного вызова.
 * @note В нашем случае она вызывается, когда буфер вывода пуст.
 * @param inputBuffer Указатель на входной буфер
 * @param outputBuffer Указатель на выходной буфер.
 * @param framesPerBuffer Размер входного/выходного буфера.
 * @param timeInfo В нашем случае это переменная не важна. А вообще она хранит информацию о времени ввода и вывода первых эл-тов буфера.
 * @param statusFlags В нашем случае это переменная не важна. А вообще она хранит информацию о изменениях входного и выходного буфера.
 * @param userData Данные переданые нами.
 */
static int callbackFunction(
                      const void *inputBuffer,
                      void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
        PaData *data = (PaData*)userData;
        float *out = (float*)outputBuffer;
    unsigned long i;

    // Записываем в буфер значения синусов.
    for(i = 0; i < framesPerBuffer; ++i) {
        *out++ = data->value_sin[data->phase];
        ++(data->phase);
        if( data->phase == data->count_point ) data->phase = 0;
    }

    // Сообщаем, что функция обратного вызова отработала без ошибок и можно продолжать.
    return paContinue;
}

int PaPrgPlay(float frequency,
         unsigned int mlsec,
         PaStreamParameters *outputParameters)
{
        PaStream *stream; //Поток, через который проигрываем звук.
        PaError err;      //Переменная для хранения ошибок.
        PaData data;      //Данные.
        int i;

        //Вычисляем сколько значений синусов нужно преподсчитать, выделяем под них память и предпосчитываем.
        data.count_point = SAMPLE_RATE / frequency;
        data.value_sin = calloc(data.count_point, sizeof(float));
        for(i = 0; i < data.count_point; ++i) {
                data.value_sin[i] = sin((i * 1.0 / data.count_point) * M_PI * 2.0);
        }
        data.phase = 0;

        //Отркываем поток для проигрывания звука.
        err = Pa_OpenStream(&stream,           //Указатель на поток.
                        NULL,              //Устройство ввода не используется.
                        outputParameters,  //Параметры устройства вывода.
                        SAMPLE_RATE,       //Частота дискретизации.
                        FRAMES_PER_BUFFER, //Размер буфера.
                        paClipOff,         //Не выводить на устройство вывода образцы вне диапазона -1.0 до 1.0.
                        callbackFunction,  //Функция обратного вызова.
                        &data);            //Данные, которые нужно передать в ф-ию обратного вызова.
        // P.S.Образец это элемент массива value_sin[].

        if( err != paNoError ) goto error_play;

        //Запускаем поток.
        err = Pa_StartStream( stream );
        if( err != paNoError ) goto error_play;

        //Ждем положенное время.
        Pa_Sleep(mlsec);

        //Останавливаем поток.
        err = Pa_StopStream( stream );
        if( err != paNoError ) goto error_play;

        //Закрываем поток.
        err = Pa_CloseStream( stream );
        if( err != paNoError ) goto error_play;

        free(data.value_sin);
        return err;
error_play:
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );

        free(data.value_sin);
        return err;
}
