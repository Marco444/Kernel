
#include "include/lib.h"

/******************************************************************************************************
 * Este ADT modela a un entero como un array de caracteres y asi maneja una
 *cantidad arbitraria de digitos, no existe el concepto de overflow como tal,
 *simplemente es salirse del tamanio de la array que internamente representa al
 *entero (pero ese error es catcheable por la funcion bigIntOverflow).
 *
 * En si dejamos un bosquejo del ADT minimo para lo que necesitariamos para el
 *fibonacci, asi como un ejemplo del codigo de la implementacion
 *
 *****************************************************************************************************/
#define BIG_INT_SIZE 200

typedef char BigInt[BIG_INT_SIZE];

void initBigInt(BigInt bigInt);

void putBigInt(BigInt bigInt, Window window);

int bigIntOverflow(BigInt previousToLast, BigInt last);

void addBigIntInto(BigInt str1, BigInt str2, BigInt str);

void copyBigIntTo(BigInt from, BigInt to);

/*
void addBigIntInto(string str1, string str2, BigInt str)
{
    // Before proceeding further, make sure length
    // of str2 is larger.
    if (strlen_(str1) > strlen_(str2))
        addBigIntInto(str2, str1, str);


    // Calculate length of both string
    int n1 = str1.length(), n2 = str2.length();
    int diff = n2 - n1;

    // Initially take carry zero
    int carry = 0;

    // Traverse from end of both strings
    for (int i=n1-1; i>=0; i--)
    {
        // Do school mathematics, compute sum of
        // current digits and carry
        int sum = ((str1[i]-'0') +
                   (str2[i+diff]-'0') +
                   carry);
        str.push_back(sum%10 + '0');
        carry = sum/10;
    }

    // Add remaining digits of str2[]
    for (int i=n2-n1-1; i>=0; i--)
    {
        int sum = ((str2[i]-'0')+carry);
        str.push_back(sum%10 + '0');
        carry = sum/10;
    }

    // Add remaining carry
    if (carry)
        str.push_back(carry+'0');

    // reverse resultant string
    reverse(str.begin(), str.end());
}
*/

/*  Ejemplo de implementacion para el fibonacci:

    BigInt previousToLast[BIG_INT_SIZE];
    initBigInt(previousToLast);
    putBigInt(previousToLast, window);
    newLine(window);

    BigInt last[BIG_INT_SIZE];
    initBigInt(last);
    putBigInt(last, window);
    newLine(window);

    BigInt current[BIG_INT_SIZE];
    initBigInt(current);


    while(!bigIntOverflow(previousToLast, last)) {
        addBigIntInto(previousToLast, last, current);
        copyBigIntTo(last, previousToLast);
        copyBigIntTo(current, last);
        putBigInt(current, window);
        newLine(window);
    }

    if(bigIntOverflow(previousToLast, last))
        puts_("Se corta el fibonacci porque se hubiese generado un overflow",
   window);

 */