#include<stdio.h>
#include<stdlib.h>
#include "matrices.h"
/*
*Función que contruye la matriz llenandola de ceros
*@param filas, el número de filas de la matriz
*@param columnas, el número de columas de la matriz
*@return a, la matriz ya construida
*/
int **constMatriz(int filas,int columnas)
{
	int i,j;
	int **a=NULL;
	a=(int**)malloc(sizeof(int*)*filas);

	for(i=0;i<filas;i++){
		a[i]=(int*)malloc(sizeof(int)*columnas);
    }
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			a[i][j] = rand()% 20; // Rellena la matriz con numeros entre 1-20
        }
    }
	return a;

}

/*
*Función que imprime una matriz
*@param filas, número de filas de la matriz
*@param columnas, número de columnas de la matriz
*/
void imprimirM(int **a,int filas,int columnas)
{
	int i,j;
    int k=0;
	for(i=0;i<filas;i++){
		for(j=0;j<columnas;j++){
			printf("%d ",a[i][j]);
			k++;
			if(columnas==k)
			{
				printf("\n");
				k=0;
			}
		}
	}
}
/*
*Función que multiplica dos matrices
*@param a, primera matriz dada
*@param b, segunda matriz dada
*@param aFilas, número de filas de la matriz a
*@param aColumnas, número de columnas de la matriz a
*@param bFilas, número de filas de la matriz b
*@param bColumnas, número de columnas de la matriz b
*@return c, la matriz resultante de la multiplicación
*/
int ** multiplicacion(int **a,int **b,int aFilas,int aColumnas,int bFilas,int bColumnas)
{
	int **c;
	int i,j,k;
	if(aColumnas!=bFilas)
	{
		printf("Las matrices no se pueden multiplicar\n");
		exit(0);
	}
	else
	{
		// Se construye la matriz C con las dimensions de las matrices a y b
		c=(int **)malloc(sizeof(int*)*aFilas);// Toma el número de filas en una matriz
		for(i=0;i<aFilas;i++)
		c[i]=(int *)malloc(sizeof(int)*bColumnas);

		// Se llena la matriz con ceros
		for(i=0;i<aFilas;i++){
			for(j=0;j<bColumnas;j++){
				c[i][j]=0;
            }
        }

		// Multiplicacion
		for(i=0;i<aFilas;i++){
			for(j=0;j<bColumnas;j++){
			    for(k=0;k<aColumnas;k++){
					c[i][j]+= a[i][k]* b[k][j];// k es el signo de la suma
                }
            }
        }

		return c;
	}
}

int main()
{
	int aFilas,aColumnas; // variables de las dimensiones de la matriz a
	int bFilas,bColumnas; // variables de las dimensiones de la matriz b

	int**a,**b,**c;
// Matriz a
	printf("Ingresa las dimensiones de la primera matriz:\n");
	fflush(stdout);
	scanf("%d %d",&aFilas,&aColumnas);
    a=constMatriz(aFilas,aColumnas);
// Matriz b
    printf("Ingresa las dimensiones de la segunda matriz:\n");
    fflush(stdout);
    scanf("%d %d",&bFilas,&bColumnas);
    b=constMatriz(bFilas,bColumnas);
// Matrices a y b
	printf("Matriz a:\n");	
	imprimirM(a,aFilas,aColumnas);
	printf("Matriz b:\n");
    imprimirM(b,bFilas,bColumnas);
// Matriz c
    c=multiplicacion(a,b,aFilas,aColumnas,bFilas,bColumnas);
	printf("Matriz c = a*b \n");
    imprimirM(c,aFilas,bColumnas);
    
    return 0;
}
