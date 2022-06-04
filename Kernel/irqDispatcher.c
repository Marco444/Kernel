#include <stdint.h>
#include <keyBoard.h>
#include <naiveConsole.h>
#include <lib.h>
#include <timeDriver.h>

//extern void readMemory(unsigned int * buffer, int * from, int qty);


void int_20(); 
void int_21(uint64_t * regs);


void irqDispatcher(uint64_t irq)
{

	switch (irq)
	{
	case 0:
		// Se llama a la funcion que debera ejecutar
	   //  la interrupcion que se guardo en la posicion 20 del IDT
		int_20();
		break;
	case 1:
		// Se llama a la funcion que debera ejecutar
	   //  la interrupcion que se guardo en la posicion 21 del IDT
		//int_21();
		break;
	}
	return;
}

void int_21(uint64_t * regs)
{	// Llamamos al driver del teclado para que guarde en su buffer
	// la tecla leida desde la interrupcion del mismo
	char c = readKey();
	
	// Si la tecla presionada es '=' se guarda un snapshot de los registros.
	if(scancodeLToAscii[c] == '='){
		regsSnapshot(regs);
	}

	saveBuffer(c);
}
void int_20()
{
	// timer_handler();
}

//Esta funcion lo que hace es recibir el llamado de cualquier 
// syscall
void syscalls(int fd, char *sysBuffer, int count, int num)
{
	switch (num)
	{

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

	case 122:
		// Si es la syscall 122 se ira al driver de pantalla para un print 
		// con FORMATO del fd que corresponda.
		if (fd == 0)
			ncPrintFD0_Format(sysBuffer, count); 
		if (fd == 1)
			ncPrintFD1_Format(sysBuffer, count);
		if (fd == 2)
			ncPrintFD2_Format(sysBuffer, count);
		break;

	//case 123:
		// Si es la syscall 123 tenemos que devolver en buffer lo leido en 
		// n (parametro) posiciones de memoria a partir de una direccion 
		// recibida como parametro.
	//	readMemoryTo((uint64_t *)fd, count);
	//	break;

	case 124:
		getRegsSnapshot(sysBuffer);
		break;

	case 1:
		// Si es la syscall de teclado debemos preguntar para que FD se quiero escribir
		// pues depende eso donde en la pantalla escribimos para cada uno de los casos
		// llamamos al driver de pantalla para que escriba en dicho lugar
		if (fd == 0)
			ncPrintFD0(sysBuffer); 
		if (fd == 1)
			ncPrintFD1(sysBuffer);
		if (fd == 2)
			ncPrintFD2(sysBuffer);
		break;
	case 0:
		//ncPrintFD0("copio un char del buffer");
		// si se llama a la syscall 0 esta misma es la syscall de read la cual
		// le guardara en el sysBuffer el caracter que hay en el buffer de teclado
		getBufferChar(sysBuffer);
		break;
	case 2:
		// Syscall para abrir un fd
		open(fd);
		break;
	case 3:
		// syscall para cerrar un fd
		close(fd);
		break;
	default:
		break;
	}
}