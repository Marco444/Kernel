// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Windows.h"

#include "include/commandsEngine.h"
#include "include/lib.h"
#include "include/stdio.h"
#include "include/syscalls.h"

void welcomeScreen() {
  // agregado luego reformatear naive console
  sysClearScreen();

  putDefaultHeader();
  puts_("Bienvenido a GDH OS! \n \n");
  puts_("Los comandos disponibes son: \n");

  commandsEngineDisplayCommands();

  newLine();
  newLine();
  puts_("Para saber mas informacion utilizar el comando man \n");
}

void listen(char *command) {
  int count = 0;
  char c = NULL_;
  putsf_(SHELL_LEGEND, CYAN);

  // Por cada letra, la guardo en el string command y valido que no sea enter,
  // si no, termine con mi comando
  while (c != '\n' && count < MAX_COMMAND_SIZE) {
    // Si es backspace, borramos la letra
    if (c == '\b') {
      if (count != 0) {
        deleteChar();
        count--;
      }
    }

    // En caso contrario, si leimos algo del buffer, se agrega la letra al
    // comando y se la imprime en pantalla En caso contrario, se agrega la letra
    // al comando y se la imprime en pantalla
    else if (c != NULL_) {
      command[count++] = c;
      putc_(c);
    }

    getKey(&c);
  }

  command[count++] = NULL_;

  // El enter ingresado:
  putc_('\n');
}

void cleanString(char *string) {
  for (int i = 0; i < MAX_COMMAND_SIZE; i++) {
    string[i] = 0;
  }
}

void windowStart() {
  welcomeScreen();

  char command[MAX_COMMAND_SIZE];

  while (1) {
    listen(command);
    commandsEngineHandle(command);
    cleanString(command);
  }
}
