// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Windows.h"
#include "include/WindowsEngine.h"
#include "include/commandsEngine.h"
#include "include/lib.h"
#include "include/stdio.h"

void welcomeScreen(Window window) {
  puts_("Bienvenido a GHD OS! \n \n", window);
  puts_("Los comandos disponibes son: \n", window);

  commandsEngineDisplayCommands(window);

  newLine(window);
  newLine(window);
  puts_("Para saber mas informacion utilizar el comando man \n", window);
}

void listen(char *command, Window window) {

  printHeader("                                     GDH OS                     "
              "                ",
              CYAN_BACKGROUND | WHITE);

  int count = 0;
  char c = NULL_;
  putsf_(SHELL_LEGEND, CYAN, window);
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

  command[count++] = NULL_;

  // El enter ingresado:
  putc_('\n', window);
}

void windowStart(Window window) {

  sysOpen(window);

  welcomeScreen(window);

  char command[MAX_COMMAND_SIZE];

  while (1) {
    listen(command, window);
    commandsEngineHandle(command, window);
  }
}
