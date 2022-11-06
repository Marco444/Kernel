// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "include/commandsEngine.h"

#include "include/_stdlib.h"
#include "include/argumentsEngine.h"
#include "include/commandsLists.h"
#include "include/constants.h"
#include "include/lib.h"
#include "include/stdio.h"
#include "include/syscalls.h"

#define MSG_ERROR_DUP2 "Error duplicating fd \n"
#define MSG_ERROR_PIPE "Fallo la creacion del pipe \n"

void commandsEngineHandle(char *command) {
  if (command == NULL_ || isLongerThan(command, MAX_COMMAND_SIZE)) return;

  if (isPipeCommand(command))
    commandsEngineRunPipe(command);
  else
    commandsEngineRun(command);
}

static inline void dup2Check(int fd, int newfd) {
  if (dup2(fd, newfd)) perror(MSG_ERROR_DUP2);
}

void pipeHandler(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  int fd[2];

  if (pipe(fd)) perror(MSG_ERROR_PIPE);

  dup2Check(STDOUT, fd[0]);

  commandsEngineRun(argv[1]);

  dup2Check(STDOUT, STDOUT);

  dup2Check(STDIN, fd[1]);

  commandsEngineRun(argv[2]);

  dup2Check(STDIN, STDIN);

  exit_();
}

void commandsEngineRunPipe(char *command) {
  // MEJORARLO, NOTABLEMENTE INDICE QUE CARGO A cmds!

  // defino dos buffers para copiar los dos argumentos del comando
  char cmds[3][MAX_ARGUMENT];

  int i = 0, dim1 = 0, dim2 = 0;

  // copio todos los caracteres antes del pipe
  while (command[i] != NULL_ && command[i] != PIPE)
    cmds[1][dim1++] = command[i++];

  cmds[1][dim1] = NULL_;

  // no copio el pipe a ningun comando
  i++;

  // copio hasta el final de la string al segundo comando
  while (command[i] != NULL_) cmds[2][dim2++] = command[i++];

  cmds[2][dim2] = NULL_;

  // creo el proceso que va a
  int childPid = loadProcess(pipeHandler, 3, cmds, FOREGROUND, "pipeHandler");
  waitPid(childPid);
}

void commandsEngineRun(char *command) {
  // remuevo los espacios y tabs que rodean al comando
  command += removeTrailingSpaces(command);

  int isBackground = (command[0] == '&');

  // borro el ampersand si es que existe
  command += isBackground;

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

      char args[MAX_ARGUMENT_COUNT][MAX_ARGUMENT];
      int argc = argumentsEngineHandle(command, args);

      // Por ultimo, cargo el puntero a funcion a la tabla de
      // context switching del kernel a traves de la syscall
      // que ejecuta loadProcess
      CommandPtr cmd = commands[i].apply;

      int childPid =
          loadProcess(cmd, argc, args, isBackground, commands[i].name);

      if (!isBackground) waitPid(childPid);
    }
  }

  if (!found) puts_(INVALID_MSG);
  return;
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
    if (command[i] == PIPE) return 1;

  return 0;
}
