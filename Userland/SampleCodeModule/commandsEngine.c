// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/commandsEngine.h"
#include "include/WindowsEngine.h"
#include "include/_stdlib.h"
#include "include/commandsLists.h"
#include "include/constants.h"
#include "include/lib.h"
#include "include/stdio.h"
#include <stdio.h>

void commandsEngineHandle(char *command) {

  // chequeo default de NULL_ y el tamaño del comando a leer
  // obs. utilizo isLongerThan y no un strlen por el hecho que no se el
  // size del input y podria presentar una vulnerabilidad de overflow
  if (command == NULL_ || isLongerThan(command, MAX_COMMAND_SIZE))
    return;

  // como el pipe es un comando especial, se maneja diferente
  if (isPipeCommand(command))
    commandsEngineRunPipe(command);

  // le digo al engine de comandos que lo corra en la window
  else {

    commandsEngineRun(command);
  }
}

void commandsEngineRunPipe(const char *command) {

  // defino dos buffers para copiar los dos argumentos del comando
  // pipe, observar que ya chequee que entra en su enteridad la
  // string command en un array de MAX_COMMAND_SIZE
  char cmd1[MAX_COMMAND_SIZE], cmd2[MAX_COMMAND_SIZE];

  int i = 0, dim1 = 0, dim2 = 0;

  // copio todos los caracteres antes del pipe
  while (command[i] != NULL_ && command[i] != PIPE)
    cmd1[dim1++] = command[i++];

  // no copio el pipe a ningun comando
  i++;

  // copio hasta el final de la string al segundo comando
  while (command[i] != NULL_)
    cmd2[dim2++] = command[i++];

  int fd[2];
  pipe(fd);

  //
  int pid1 = commandsEngineRun(cmd1);
  dup2(pid1, STDIN, fd[0]);

  int pid2 = commandsEngineRun(cmd2);
  dup2(pid2, STDOUT, fd[1]);
}

int commandsEngineRun(char *command) {

  // remuevo los espacios y tabs que rodean al comando
  command += removeTrailingSpaces(command);

  int type = (command[0] == '&');

  // borro el ampersand si es que existe
  command += type;

  // puts_(command);
  // newLine(window);

  int found = 0;

  // voy por todos los comandos y chequeo que comando lo tengo
  // como una substring de lo pasado como argumento que pase,
  // ejemplo command: "printMem 500", chequeo si "printMem"
  // es una substring comenzando en indice cero (aca entraria
  // un trie)
  for (int i = 0; i < COMMANDS_COUNT; i++) {
    if (substring(command, commands[i].name) == 0) {

      found = 1;

      // guardo el offset para tener donde comienzan los
      // argumentos de mi comando
      int argumentsBeginAtOffset = strlen_(commands[i].name);
      command += argumentsBeginAtOffset;

      // Defino los argvs con memoria dinamica porque asi puedo
      // leer los argumentos estando en background
      char args[MAX_ARGUMENT_COUNT][MAX_ARGUMENT];

      int argc = argumentsEngineHandle(0, command, args);

      for (int i = 0; i < argc; i++) {
        puts_(args[i]);
        newLine();
      }
      // Por ultimo, cargo el puntero a funcion a la tabla de
      // context switching del kernel a traves de la syscall
      // que ejecuta loadProcess
      CommandPtr cmd = commands[i].apply;
      return loadProcess(cmd, 0, argc, args, type, commands[i].name);
    }
  }

  if (!found)
    puts_(INVALID_MSG);
  return -1;
}

void printCommand(char *name) {
  putsf_(name, LIGHT_CYAN);
  putsf_(", ", WHITE);
}

void commandsEngineDisplayCommands() {

  // imprimo todos los comandos normales
  for (int i = 0; i < COMMANDS_COUNT; ++i) {
    printCommand(commands[i].name);
  }
}

int isPipeCommand(const char *command) {

  for (int i = 0; command[i] != NULL_; i++)
    if (command[i] == PIPE)
      return 1;

  return 0;
}
