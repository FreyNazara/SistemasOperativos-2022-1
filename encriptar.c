#include <stdio.h>
#include <stdlib.h>
#include "encriptar.h"

int main()
{
char mensaje[100];
int llave;
printf("Ingresa el mensaje a cifrar: ");
fgets(mensaje, sizeof(mensaje), stdin);
printf("Ingresa la llave: ");
scanf("%d", &llave);
char * result = encriptar(mensaje, llave);
printf("El texto cifrado es: %s\n", result);
return 0;
}

char * encriptar(char* msg, int k) 
    {
    unsigned char caracter;
    int llave = k;
    int i;
    for(i = 0; msg[i] != '\0'; ++i)
    {
        caracter = msg[i];
        if(caracter >= 'a' && caracter <= 'z')
        {
            caracter = caracter + llave;
            if(caracter > 'z')
            {
                caracter = caracter - 'z' + 'a' - 1;
            }
            msg[i] = caracter;
        }
        else if(caracter >= 'A' && caracter <= 'Z')
        {
            caracter = caracter + llave;
            if(caracter > 'Z')
            {
                caracter = caracter - 'Z' + 'A' - 1;
            }
            msg[i] = caracter;
        }
    }
    return msg;
}