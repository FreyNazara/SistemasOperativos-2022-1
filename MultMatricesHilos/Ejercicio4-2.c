#include "Ejercicio4-2.h"

/**
 * Imprime en consola una matriz con un número arbitrario
 * de filas y columnas. Los elemntos de la matriz deben ser 
 * números enteros.
 */
void toString(int** mat, int filas, int cols){
    for(int i = 0; i < filas; i++){
        for(int j = 0; j < cols; j++){
            printf(" %d ", mat[i][j]);
        }
        printf("\n");
    }
}

/**
 * Nos devuelve el producto punto de dos vectores. Utilizamos este
 * método para crear las entradas (i,j) del resultado
 * de multiplicar dos matrices.
 *
 * Los dos vectores se encuentran en un solo arreglo, uno seguido 
 * del otro. Los elementos que se encuentran en la primera mitad
 * del arreglo mat son el vector Ai y los demas elementos son el vector Bj.
 */
void* productoPunto(void* mat){
    int* vectores = (int*)mat;
    int mitad = vectores[0];
    int dot = 0;
    for(int i = 1; i < mitad+1; i++)
        dot += vectores[i]*vectores[i+mitad];
    
    // Almacenamos el valor de dot en una locación de memoria con puntero retval. 
    // Esto porque usamos el método pthread_exit() para regresar dicho valor encontrado,
    // pero ese método toma como parámetro un puntero.
    int* retval = (int*)malloc(sizeof(int));
    *retval = dot;

    // Utilizamos el método pthread_exit() para terminar la ejecución del hilo
    // actual, o el que llamó a éste método. Como parámetro le pasamos el producto
    // punto de los vectores en mat, el cual será recuperado al llamar al método 
    // pthread_join() con éste hilo, y lo podremos usar para ir rellenando las
    // entradas de la matriz resultado.
    pthread_exit(retval);
}


/**
 * Método que se encarga de multiplicars dos matrices A y B. 
 * Como sabemos, para que A y B puesan ser multiplicadas, el número de columnas
 * de A debe ser igual al número de filas de B.
 */
void multiplicaMatrices(int** A, int** B, int filasA, int filasB, int colsA, int colsB){

    // Para poder trabajar con matrices cuyo tamaño no se conoce en tiempo de
    // compilación, lo que hacemos es reservar espacio en memoria de manera
    // dinámica, el cual usaremos para guardar los elementos de la matriz
    // resultado C */
    int **C;

    C = malloc(filasA * sizeof(*C));
    for(int i = 0; i < filasA; i++){
        C[i] = malloc(colsB * sizeof(*C[i]));
    }

    for(int i = 0; i < filasA; i++){
        for(int j = 0; j < colsB; j+=4){

            // Es recomendado que en un programa se utilice a lo más un hilo
            // por cada núcleo del CPU de la computadora. En mi caso, mi computadora
            // tiene 4 nucleos, por eso tenemos a lo más 4 hilos ejecutándose a la vez.
            int numHilos = colsB-j >= 4 ? 4 : colsB-j;

            pthread_t* hilos = (pthread_t*)malloc(numHilos*sizeof(pthread_t));

            for(int H = 0; H < numHilos; H++){

                // Usando en hint que se proporciona en la tarea, hacemos que cada hilo
                // se ocupe de encontrar alguna entrada (i,j) de la matriz resultado.

                // Lo que hacemos aquí es crear un vector que contenga los elementos 
                // a los que se les hará el producto punto para encontar C[i][j],
                // es decir, la fila i de A y la columna B de j.

                // Tenemos que hacer esta codificación de poner ambos vectores en uno
                // solo, porque el método productoPunto recibe un único argumento.
                int* Cij = (int*)malloc(2*colsA*sizeof(int)+1);

                for(int k = 0; k < colsA; k++){
                    Cij[k+1] = A[i][k];
                    Cij[k+colsA+1] = B[k][j+H];
                }

                // Recordemos que tenemos dos vectores diferentes en Cij, digamos Ai y Bj, 
                // por lo que debemos saber en qué parte termina uno y empieza el otro. 
                // Dichos vectores tienen el mismo tamaño, por lo que podriamos pensar
                // que basta con saber cuanto es la mitad de Cij. El problema es que 
                // C no permite calcular el número de elementos de un vector
                // asignado dinámicamente, y el método productoPunto solo recibe un 
                // parámetro. Es por esto que necesitamos poner también la longitud de
                // los vectores Ai y Bj en Cij, para poder saber exactamente qué entradas
                // de Cij le corresponden a cada uno de ellos.
                Cij[0] = colsA;

                // Lanzams los hilos. Cada uno se encarga de encontrar el valor de alguna
                // entrada de C, utilizando el método productoPunto. Podemos ver ahora
                // por qué dicho método solo podia recibir un argumento.
                pthread_create(&hilos[H], NULL, productoPunto, (void*)Cij);
            }

            for(int H = 0; H < numHilos; H++){
                void* dot;

                // Hacemos join a cada uno de los hilos creados. El valor de regreso del método
                // pthread_exit() se almacena en la localidad de memoria &dot. Dicho
                // valor es claramente la entrada de la matriz C que el hilo tuvo que
                // calcular.
                pthread_join(hilos[H], &dot);
                C[i][j+H] = *((int*)dot);
            }
        }
    }

    // Recordemos que nuestra matriz resultado se almacenó en memoria de manera dinámica
    // asi que cuando la terminemos de usar debemos liberar la memoria que ocuapa.
    // Para evitar problemas con este asunto, preferimos hacer que este método no regrese
    // la matriz resultado, sino que simplemente la imprima. De esta manera podemos
    // liberar la memoria que ocupa dentro del mismo método, facilitandonos la
    // implementación del programa.
    printf("Matriz resultado: \n");
    toString(C, filasA, colsB);

    for(int i = 0; i < filasA; i++)
        free(C[i]);
    free(C);    
}



int main(){

    // Si el usuario así lo quiere, puede cambiar el tamaño de las matrices que se van
    // a multiplicar. Siempre cuidando que el número de columnas de A sea igual al número
    // de filas de B */

    int filasA = 4;
    int colsA = 4;

    int filasB = 4;
    int colsB = 3;


    // Creamos nuestras matrices reservando memoria de manera dinámica para 
    // cada una, utilizando el método malloc.
    // Claremente, las matrices aun no tienem valores en sus entradas. */
    int **A;
    int **B;
    int **C;

    A = malloc(filasA * sizeof(*A));
    for(int i = 0; i < filasA; i++){
        A[i] = malloc(colsA * sizeof(*A[i]));
    }

    B = malloc(filasB * sizeof(*B));
    for(int i = 0; i < filasB; i++){
        B[i] = malloc(colsB * sizeof(*B[i]));
    }

    // C tiene el mismo tamaño que A.
    C = malloc(filasA * sizeof(*C));
    for(int i = 0; i < filasA; i++){
        C[i] = malloc(colsA * sizeof(*C[i]));
    }

    // Para llenar nuestras matrices podemos usar ciclos for, con variables
    // de iteracion i,j, haciendo que las entradas (i,j) de la matriz estén
    // en función de estos dos valores. */
    for(int i = 0; i < filasA; i++){
        for(int j = 0; j < colsA; j++){
            A[i][j] = i+j;
        }
    }

    // O la podemos llenar con número aleatorios, usando el método rand().
    for(int i = 0; i < filasA; i++){
        for(int j = 0; j < colsA; j++){
            // Los números serán menores a 20:
            C[i][j] = rand() % 20;
        }
    }

    // O podemos especificar los valores de la matriz entrada a entrada. 
    B[0][0] = 1; B[0][1] = 3; B[0][2] = 7;
    B[1][0] = 4; B[1][1] = 0; B[1][2] = 6;
    B[2][0] = 4; B[2][1] = 0; B[2][2] = 6;
    B[3][0] = 1; B[3][1] = 3; B[3][2] = 7;

    printf("Matriz A:\n");
    toString(A, filasA, colsA);

    printf("\nMatriz B:\n");
    toString(B, filasB, colsB);

    printf("\nMatriz C:\n");
    toString(C, filasA, colsA);

    // Hacemos la multiplicación de A y B pasando cuantas filas y columnas
    // tiene cada uno, respetando el orden especificado en la firma
    // del método.
    printf("\nMultiplicación de A por B\n");
    multiplicaMatrices(A, B, filasA, filasB, colsA, colsB);

    // Hacemos la multiplicación de A por C;
    printf("\nMultiplicación de A por C\n");
    multiplicaMatrices(A, C, filasA, filasA, colsA, colsA);

    // Hacemos la multiplicación de C por A, mostrando que la mult de matrices
    // no es conmitativa.
    printf("\nMultiplicación de C por A\n");
    multiplicaMatrices(C, A, filasA, filasA, colsA, colsA);

    // Por ultimo, muy importante: debemos liberar el espacio ocupado por nuestras matrices:
    for(int i = 0; i < filasA; i++)
        free(A[i]);
    free(A);

    for(int i = 0; i < filasB; i++)
        free(B[i]);
    free(B);

    for(int i = 0; i < filasA; i++)
        free(C[i]);
    free(C);
}