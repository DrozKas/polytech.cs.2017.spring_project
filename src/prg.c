/**
 * @file prg.c
 */

#include "paprg.h"

/**
 * @brief Входная точка программы
 * @return статус (возвратный код) программы.
 */
int main(void)
{
    // Считываем путь к файлу с данными.
        char path[FILENAME_MAX];
        printf("Enter path to file:");
        fgets(path, sizeof(path), stdin);
        if (strlen(path) && path[strlen(path) - 1] == '\n') path[strlen(path)-1] = 0;

        //Открываем файл с данными.
        FILE *f = fopen(path, "r");
        if (f == NULL) {
                fprintf(stderr,"Error.\n");
                fprintf(stderr,"Could not open file: %s\n", path);
                return -1;
        }

        //Инициализируем PortAudio.
	PaError err = Pa_Initialize();
	if( err != paNoError ) goto error_main;

        PaStreamParameters outputParameters; //Через эту переменную будем настfраивоть устройство вывода.

        outputParameters.device = Pa_GetDefaultOutputDevice(); //Получаем стандартное устройство вывода.
	if (outputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default output device.\n");
		goto error_main;
	}
        outputParameters.channelCount = 1;          //Одноканальный звук.
        outputParameters.sampleFormat = paFloat32;  //Образцы это элементы типа float.
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device )->defaultLowOutputLatency; //Настраиваем задержку.
        outputParameters.hostApiSpecificStreamInfo = NULL; //Дополнительных настроек нет.

	float frequency = 0;
	int mlsec = 0;

        //Считываем частоты и длительность.
	fscanf(f, "%f%d", &frequency, &mlsec);
	while (frequency != -1 && mlsec != -1) {

                PaPrgPlay(frequency, mlsec, &outputParameters);
		fscanf(f, "%f%d", &frequency, &mlsec);
	}
	fclose(f);
        //Завершаем работу с библиотекой PortAudio.
	Pa_Terminate();
	return 0;

error_main:
	Pa_Terminate();
	fprintf( stderr, "Error number: %d\n", err );
	fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
	return -1;
}
