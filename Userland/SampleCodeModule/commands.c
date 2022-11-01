// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/commands.h"
#include "include/WindowsEngine.h"
#include "include/_stdlib.h"
#include "include/_string.h"
#include "include/commandsEngine.h"
#include "include/commandsLists.h"
#include "include/lib.h"
#include "include/stdio.h"
#include "include/syscalls.h"
#include "include/testManager.h"
#include <stdio.h>
#include <string.h>

#define INVALID_ARGUMENT_NUMBER "No ingreso el numero de argumentos validos \n"
#define INVALID_ARGUMENTS "No ingreso el tipo de argumentos validos \n"
#define TIME_BUFFER 50

void verifyArguments(int received, int expected) {
  if (received != expected) {
    puts_(INVALID_ARGUMENT_NUMBER);
    exit(0);
  }
}

void kill(Window window, int argc,
          char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 2);
  sysKillProcess(atoi_(argv[0]));
  exit(window);
}

void nice(Window window, int argc,
          char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 3);
  // sysNiceProcess(atoi_(**argv[1]), atoi_(argv[1]));
  exit(window);
}

void block(Window window, int argc,
           char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 2);
  sysBlockProcess(atoi_(argv[0]));
  exit(window);
}

void ps(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  verifyArguments(argc, 0);
  psdump();
  exit(window);
}

void mem(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);
  memorydump(window);
  exit(window);
}
void pipes(Window window, int argc,
           char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);
  pipesdump();
  exit(window);
}

void testManager(Window window, int argc,
                 char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  testManagerRun(window, argc, argv);
  exit(window);
}

void man(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  verifyArguments(argc, 2);

  int found = 0;

  for (int i = 0; i < COMMANDS_COUNT && !found; i++) {
    if (strcmp_(argv[1], commands[i].name) == 0) {
      puts_(" ");
      printCommand(commands[i].name);
      puts_(commands[i].description);
      newLine();
      found = 1;
    }
  }

  if (!found)
    puts_("No se encontro el comando, intente de nuevo \n");

  exit(window);
}

void help(Window window, int argc,
          char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);

  puts_("La lista de los comandos disponibles es: \n");

  commandsEngineDisplayCommands();
  windowsEngineDisplayControls(window);
  newLine();

  exit(window);
}

void diaYHora(Window window, int argc,
              char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);

  char buffer[TIME_BUFFER];
  getTime(buffer);
  puts_(buffer);
  puts_("\n");
  exit(window);
}

void divideByZero(Window window, int argc,
                  char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);
  puts_("Intentamos dividir por cero...");
  divideByZeroAsm();
  puts_("Luego de la excepcion continuo con el programa \n");
  exit(window);
}

void invalidOpCode(Window window, int argc,
                   char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  verifyArguments(argc, 0);
  puts_("Intentamos un invalid op code...");
  generateInvalidOpCode();
  puts_("Luego de la excepcion continuo con el programa \n");
  exit(window);
}

void printMem(Window window, int argc,
              char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 2);
  puts_("La informacion desde la posicion de memoria ");
  putHex(atoh_(argv[1]));
  newLine();

  printMemFrom((unsigned int)atoh_(argv[1]), 0);

  exit(window);
}

extern long *getRegs();

void infoReg(Window window, int argc,
             char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);

  static char *registerNames[REGS_CANT + 1] = {
      "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ",
      "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "FLAGS"};

  puts_("Los registros tienen los valores: \n");

  uint64_t buffer[18];
  sysGetRegsSnapshot(buffer);

  for (int i = 0; i < REGS_CANT; i++) {
    // if(i%6 == 0 && i !=0)
    // puts_("\n");
    puts_(registerNames[i]);
    puts_(": ");
    putHex(buffer[i]);
    puts_("h");
    puts_("\n");
  }

  puts_(registerNames[REGS_CANT]);
  puts_(": ");
  putBin(buffer[REGS_CANT]);
  puts_("b");
  puts_("\n");

  puts_("\n");
  exit(window);
}

void primos(Window window, int argc,
            char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);

  int num = 0;

  while (1) {
    if (isPrime(num)) {
      // putInteger(num);
      // newLine();
    }
    num++;
  }

  exit(window);
}

void fibonacci(Window window, int argc,
               char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);
  // Observacion: el mejor manejo de overflow podriamos almacenar los numeros
  // como strings, definiendo digamos 300 digitos, y asi poder evitar cualquier
  // tipo de overflow. como manejo.

  long previousToLast = 0;
  putInteger(previousToLast);
  newLine();

  long last = 1;
  putInteger(last);
  newLine();

  long current;

  while (!integerOverflowAddition(last, current)) {
    current = last + previousToLast;
    previousToLast = last;
    last = current;
    putInteger(current);
    newLine();
  }

  if (integerOverflowAddition(last, current))
    puts_("Se corta el fibonacci porque se hubiese generado un overflow \n");

  exit(window);
}

void clear(Window window, int argc,
           char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

  verifyArguments(argc, 0);

  sysClearScreen(window);
  exit(window);
}
