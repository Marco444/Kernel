// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyBoard.h>
#include <lib.h>
#include <mmuEngine.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <timeDriver.h>

#include "include/fileDescriptorManager.h"
#include "include/semaphores.h"

extern void readMemory(char *buffer, int from, int qty);

void int_20();
void int_21(uint64_t *regs);

void irqDispatcher(uint64_t irq) {
  switch (irq) {
    case 0:
      // Se llama a la funcion que debera ejecutar
      //  la interrupcion que se guardo en la posicion 20 del IDT
      int_20();
      break;
    case 1:
      // Se llama a la funcion que debera ejecutar
      // la interrupcion que se guardo en la posicion 21 del IDT

      // La rutina es llamda directamente desde assembler
      // int_21();
      break;
  }
  return;
}

void int_21(uint64_t *regs) {  // Llamamos al driver del teclado para que guarde
                               // en su buffer
  // la tecla leida desde la interrupcion del mismo
  int c = readKey();

  // Si la tecla presionada es '=' se guarda un snapshot de los registros.
  if (getValue(c) == '=') {
    regsSnapshot(regs);
  }

  saveBuffer(c);
}
void int_20() {
  // timer_handler();
}

// Esta funcion lo que hace es recibir el llamado de cualquier
//  syscall
void syscalls(int fd, char *sysBuffer, int count, int num) {
  switch (num) {
    case 120:
      // Si es la syscall 120 se ira al driver de la hora para que le copie
      // en el buffer que formato de dicha hora
      time_syscall(sysBuffer);
      break;

    case 121:
      // Si es la syscall 121 se ira al driver de pantalla para hacer un clear
      // del fd que corresponda.
      fdClear(fd);
      break;

    case 123:
      readMemory(sysBuffer, fd, count);
      // case 123:
      //  Si es la syscall 123 tenemos que devolver en buffer lo leido en
      //  n (parametro) posiciones de memoria a partir de una direccion
      //  recibida como parametro.
      //	readMemoryTo((uint64_t *)fd, count);
      break;

    case 124:
      // Si es la syscall 124 se hara una copia de los registros capturados
      // previamente al buffer.
      getRegsSnapshot((uint64_t *)sysBuffer);
      break;

    case 125:
      // Si es la syscall 125 se ira al driver de pantalla para un print
      // con FORMATO del Header de pantalla.
      ncPrintHeader(sysBuffer, count);
      break;

    case 126:
      semOpen(fd, sysBuffer);
      break;
    case 127:
      semClose(fd);
      break;
    case 128:
      semWait(fd);
      break;
    case 129:
      semSignal(fd);
      break;
    case 130:
      semState();
      break;
    case 1:
      sysWriteFormat(fd, sysBuffer, count);
      break;
    case 0:
      sysRead(fd, sysBuffer);
      break;
    default:
      break;
  }
}
