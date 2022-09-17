#include "include/commands.h"
#include "include/commandsEngine.h"
#include "include/lib.h"
#include "include/WindowsEngine.h"

#define INVALID_ARGUMENT_NUMBER "No ingreso el numero de argumentos validos \n"
#define INVALID_ARGUMENTS "No ingreso el tipo de argumentos validos \n"
#define TIME_BUFFER 50


void man(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if(argc != 2) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }

    int found = 0;

    for(int i = 0; i < COMMANDS_COUNT && !found; i++) {
        if(strcmp_(argv[1], commands[i].name) == 0) {
            printCommand(window, commands[i].name, commands[i].description);
            found = 1;
        }
    }

    if(!found) 
        puts_("No se encontro el comando, intente de nuevo \n", window);
    
    exit();
}

void help(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }

    puts_("La lista de los comandos disponibles es: \n", window);
    commandsEngineDisplayCommands(window);
    windowsEngineDisplayControls(window);
    newLine(window); 

    exit();
}

void diaYHora(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
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
        puts_(INVALID_ARGUMENT_NUMBER, window);
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
        puts_(INVALID_ARGUMENT_NUMBER, window);
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
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }

    puts_("La informacion desde la posicion de memoria ", window);
    putHex(atoh_(argv[1]), window);
    newLine(window);

    printMemFrom((unsigned int *) atoh_(argv[1]), window);
    
    exit();
}

extern long * getRegs();

void infoReg(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }

    static const char *registerNames[REGS_CANT + 1] = {
            "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13",
            "R14", "R15", "RIP", "FLAGS"
    };

    puts_("Los registros tienen los valores: \n", window);

    uint64_t buffer[18];
    sysGetRegsSnapshot(buffer);


    for(int i = 0; i < REGS_CANT; i++){
        //if(i%6 == 0 && i !=0)
            //puts_("\n", window);
        puts_(registerNames[i], window);
        puts_(": ", window);
        putHex(buffer[i], window);
        puts_("h", window);
        puts_("\n", window);
        
    }

    puts_(registerNames[REGS_CANT], window);
    puts_(": ", window);
    putBin(buffer[REGS_CANT], window);
    puts_("b", window);
    puts_("\n", window);


    puts_("\n", window);
    exit();
}


void primos(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
    if (argc != 0) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
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
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }

    //Observacion: el mejor manejo de overflow podriamos almacenar los numeros como strings,
    //definiendo digamos 300 digitos, y asi poder evitar cualquier tipo de overflow.
    //como manejo.

    long previousToLast = 0;
    putInteger(previousToLast, window);
    newLine(window);
    
    long last = 1;
    putInteger(last, window);
    newLine(window);

    long current;

    while(!integerOverflowAddition(last, current)) {
        current = last + previousToLast;
        previousToLast = last;
        last = current;
        putInteger(current, window);
        newLine(window);
    }

    if(integerOverflowAddition(last, current)) 
        puts_("Se corta el fibonacci porque se hubiese generado un overflow \n", window);


    exit();
}

void clear(Window window, int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]){
    if (!(argc == 0 || argc == 1 || argc == 2)) {
        puts_(INVALID_ARGUMENT_NUMBER, window);
        exit();
        return;
    }
    sysClearScreen(window);
    exit();
}
