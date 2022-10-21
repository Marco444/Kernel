// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Windows.h"
#include "include/WindowsEngine.h"
#include "include/commandsEngine.h"
#include "include/lib.h"

/*
A FUTURO HACER DOBLE TERMINAL
**/

void welcomeScreen(Window window) {
  puts_(" Bienvenido a GHD OS! \n \n", window);
  puts_(" Los comandos disponibes son: \n", window);
  commandsEngineDisplayCommands(window);
  windowsEngineDisplayControls(window);
  putsf_("\n                                                                   "
         "             ",
         WHITE, window);
}

void listen(char *command, Window window) {

  printHeader("                                     GDH OS                     "
              "                ",
              CYAN_BACKGROUND | WHITE);

  // Contador de caracteres ingresados por el usuario
  int count = 0;

  // Caracter ingresado por vez
  char c = NULL_;

  // Se imprime la leyenda que indica el usuario y el dispositivo
  putsf_(SHELL_LEGEND, CYAN, window);

  // leo la key
  getKey(window, &c);

  // Por cada letra, la guardo en el string command y valido que no sea enter,
  // si no, termine con mi comando
  while (c != '\n' && count < MAX_COMMAND_SIZE) {

    // Si es backspace, borramos la letra
    if (c == '\b') {
      if (count != 0) {
        deleteChar(window);
        count--;
      }
    }

    // En caso contrario, si leimos algo del buffer, se agrega la letra al
    // comando y se la imprime en pantalla En caso contrario, se agrega la letra
    // al comando y se la imprime en pantalla
    else if (c != NULL_) {
      command[count++] = c;
      putc_(c, window);
    }

    getKey(window, &c);
  }

  // hago la null termination de la string
  command[count++] = NULL_;

  // El enter ingresado:
  putc_('\n', window);
}

void windowStart(Window window) {

  sysOpen(window);

  // Se imprime un mensaje de bienvenida al iniciar la ventana
  welcomeScreen(window);

  // Arreglo de caracteres donde tendre mi comando
  char command[MAX_COMMAND_SIZE];

  while (1) {

    // Se escucha lo que el usuario escriba hasta el enter
    listen(command, window);

    // Se pasa el comando, y este decidira que hacer con tal comando, si es que
    // es valido.
    commandsEngineHandle(command, window);
  }
}
