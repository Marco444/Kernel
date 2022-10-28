// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "./include/MemoryManager.h"
#include <defs.h>
#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <schedluerEngine.h>
#include <stdint.h>
#include <string.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
static const uint64_t PageSize = 0x1000;
static void *const sampleCodeModuleAddress = (void *)0x400000;
static void *const sampleDataModuleAddress = (void *)0x500000;
static void *const startHeapAddres = (void *)0xF00000;
static char *shellName = "SHELL";
typedef int (*EntryPoint)();
extern void write_();
extern void _hlt();
extern void _cli();
void clearBSS(void *bssAddress, uint64_t bssSize) {
  memset(bssAddress, 0, bssSize);
}

void *getStackBase() {
  return (void *)((uint64_t)&endOfKernel +
                  PageSize * 8       // The size of the stack itself, 32KiB
                  - sizeof(uint64_t) // Begin at the top of the stack
  );
}

void *initializeKernelBinary() {
  char buffer[10];

  ncPrint("[x64BareBones]");
  ncNewline();

  ncPrint("CPU Vendor:");
  ncPrint(cpuVendor(buffer));
  ncNewline();

  ncPrint("[Loading modules]");
  ncNewline();
  void *moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

  loadModules(&endOfKernelBinary, moduleAddresses);
  ncPrint("[Done]");
  ncNewline();
  ncNewline();

  ncPrint("[Initializing kernel's binary]");
  ncNewline();

  clearBSS(&bss, &endOfKernel - &bss);

  ncPrint("  text: 0x");
  ncPrintHex((uint64_t)&text);
  ncNewline();
  ncPrint("  rodata: 0x");
  ncPrintHex((uint64_t)&rodata);
  ncNewline();
  ncPrint("  data: 0x");
  ncPrintHex((uint64_t)&data);
  ncNewline();
  ncPrint("  bss: 0x");
  ncPrintHex((uint64_t)&bss);
  ncNewline();

  ncPrint("[Done]");
  ncNewline();
  ncNewline();
  ncNewline();
  return getStackBase();
}

int main() {
  createMemoryManager(startHeapAddres);
  char **aux;
  initialiseContextSchedluerEngine();
  loadFirstContext(sampleCodeModuleAddress, 0, 0, aux, 1, shellName);
  load_idt();
  while (1)
    _hlt();
  ncClear();
  return 0;
}
