#ifndef WINDOWS_H
#define WINDOWS_H

#define MAX_COMMAND_SIZE 120
#define SHELL_LEGEND "user@device: ~$ "

/*
 * Enumerador con los tipos de ventanas posibles.
 * (Obs: comienzo en 1 el enum para poder hacer el chequeo
 * de null y mantener un estilo constante a traves todo el
 * codigo)
 */
typedef enum window_ { MAIN_WINDOW = 0, LEFT_WINDOW, RIGHT_WINDOW } Window;

/*
 * Recibe una ventana la cual va a escuchar al teclado e imprimir en pantalla.
 */
void listen(char *command, Window window);

/*
 * Recibe una ventana en la cual inicializara una terminal de comandos
 */
void windowStart(Window window);

#endif