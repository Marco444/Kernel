#ifndef WINDOWS_ENGINE_H
#define WINDOWS_ENGINE_H

#define MAX_ARGUMENT 20
#define MAX_ARGUMENT_COUNT 20

#include "argumentsEngine.h"

#define MAX_WINDOWS 3

#define LEFT_WINDOW_SUSPEND '1'
#define MAIN_WINDOW_SUSPEND LEFT_WINDOW_SUSPEND
#define RIGHT_WINDOW_SUSPEND '2'


#define RESUME_LEFT_WINDOW '3'
#define RESUME_MAIN_WINDOW RESUME_LEFT_WINDOW
#define RESUME_RIGHT_WINDOW '4'

#define KILL_PROCESS '\n'

/*
 * Imprime los controles de las ventanas disponibles
 */
void windowsEngineDisplayControls(Window window);

/*
 * Inicializa el arreglo de windows, con una window del tipo shell
 */
void windowsEngineInitialize(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

/*
   * Funcion la cual espera a que terminen todos los procesos para luego seguir ejecutando con la shell
*/
void waitProcess();

/*
   * Funcion la cual espera a un enter ingresado por el usuario
*/
void waitEnter();

void waitProcessPipe();

void waitProcessMain(int pid);

#endif