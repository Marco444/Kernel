#include "include/commands.h"
#include "include/commandsEngine.h"
#include "include/lib.h"


#define INVALID_NUMBER_COMMANDS "No ingreso el numero de argumentos validos \n"
#define INVALID_ARGUMENTS "No ingreso el tipo de argumentos validos \n, haga $> man "
#define TIME_BUFFER 50
#define MEM_BUFFER 120


void printUnos(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    int i = 0;
    while(1) {
        if(!(i % 100000)) puts_("1", MAIN_WINDOW);
        i++;
    }
    exit();
}

void man(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if(argc != 2) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }
/*
    if(!isLetterString(argv[1])) {
        puts_(INVALID_ARGUMENTS "man", window);
        return;
    }
*/
    puts_("Todavia no tenemos soporte de man :) \n", window);
    exit();
}

void help(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    puts_("La lista de los comandos disponibles es: \n", window);
    commandsEngineDisplayCommands(window);

    exit();
}

void diaYHora(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    char buffer[TIME_BUFFER];
    getTime(buffer);
    puts_(buffer, window);
    puts_("\n", window);
    exit();

}

void divideByZero(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }
    puts_("Intentamos dividir por cero...", window);
    divideByZeroAsm();
    puts_("Luego de la excepcion continuo con el programa \n", window);
    exit();
}

void invalidOpCode(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }
    puts_("Intentamos un invalid op code...", window);
    generateInvalidOpCode();
    puts_("Luego de la excepcion continuo con el programa \n", window);
    exit();
}

void printMem(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 2) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    //verificar es una posicion de memoria valida!

    puts_("La informacion desde la posicion de memoria ", window);
    puts_(argv[1], window);
    newLine(window);

    char buffer[MEM_BUFFER];
    readMem(buffer, (int *) atoi_((argv[1])), 32);

    for(int i = 0; i < 32; i++){
        putHex(buffer[i], window);
        puts_("\n", window);
    }
    
    puts_(buffer, window);
    exit();
}

extern long * getRegs();

void infoReg(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    static const char *registerNames[REGS_CANT] = {
            "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13",
            "R14", "R15", "RIP"
    };

    puts_("Los registros tienen los valores: \n", window);

    long * buffer = getRegs();

    for(int i = 0; i < REGS_CANT; i++){
        puts_("    -", window);
        puts_(registerNames[i], window);
        puts_(": ", window);
        putHex(buffer[i], window);
        puts_("h\n", window);
    }
    
    puts_("\n", window);
    exit();
}


void primos(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    int num = 0;

    while (1) {
        if (isPrime(num)) {
            putInteger(num, window);
            newLine(window);
        }
        num++;
    }

    exit();
}

void fibonacci(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }

    long previousToLast = 0;
    putInteger(previousToLast, window);
    newLine(window);
    
    long last = 1;
    putInteger(last, window);
    newLine(window);

    long current, i = 2;

    while(!integerOverflowAddition(last, current)) {
        current = last + previousToLast;
        previousToLast = last;
        last = current;
        putInteger(current, window);
        newLine(window);
    }

    if(integerOverflowAddition(last, current)) 
        puts_("Se corta el fibonacci porque se hubiese generado un overflow", window);

    exit();
}

void clear(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]){
    if (!(argc == 0 || argc == 1 || argc == 2)) {
        puts_(INVALID_NUMBER_COMMANDS, window);
        exit();
        return;
    }
    sysClearScreen(window);
    exit();
}