// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/Windows.h"

#include "include/commandsEngine.h"
#include "include/lib.h"
#include "include/stdio.h"
#include "include/syscalls.h"

void welcomeScreen() {
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

  while (c != '\n' && count < MAX_COMMAND_SIZE) {
    if (c == '\b') {
      if (count != 0) {
        deleteChar();
        count--;
      }
    } else if (c != NULL_) {
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
