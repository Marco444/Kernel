
#include <stdint.h>

#define MAX_UNSIGNED_LONG_LONG 4294967295
#define MAX_INT 2147483647
#define NULL_ 0L

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);

/*
 * Devuelve si se genera un overflow al sumar los dos enteros
 */
int integerOverflowAddition(int a, int b);

/*
 * Devuelve si se genera un overflow al sumar los dos unsigned long
 */
int longOverflowAddition(unsigned long long a, unsigned long long b);

/*
 * Recibe una string y un tamaño, devuelve verdadero
 * si la string tiene al menos el tamño size. Obs: no uso
 * un strlen ya que puedo llegar una vulnerabilidad desde
 * el punto de overflow.
 */
int isLongerThan(const char *s, int size);

/*
 * Retorna si el numero es un primo o no
 */
int isPrime(int num);
