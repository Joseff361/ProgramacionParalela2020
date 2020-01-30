#include <iostream>
#include <cstdlib>
#include <ctime>
#include <mpi.h>

using namespace std;

int main(int argc, char * argv[]) {
	int filas,columnas;
    int rank_sz,
            my_rank;
    int **A, // Matriz a multiplicar
            *x, // Vector que vamos a multiplicar
            *y, // Vector donde almacenamos el resultado
            *miFila, // La fila que almacena localmente un proceso
            *comprueba; // Guarda el resultado final (calculado secuencialmente), su valor
                        // debe ser igual al de 'y'

    double tInicio, // Tiempo en el que comienza la ejecucion
            tFin; // Tiempo en el que acaba la ejecucion

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &rank_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == 0) {
        cout<<"Digite las dimensiones de la matriz a operar "<<endl;
    	cout<<"Numero de filas: ";cin>>filas;
    	cout<<"Numero de columnas: ";cin>>columnas;
        if (filas > rank_sz){
        	filas=rank_sz;
        	cout << "Cantidad filas cambiada a " << filas << endl;
        }
        filas=rank_sz;
    }

    MPI_Bcast(&filas, 1,MPI_INT,  0,  MPI_COMM_WORLD);
    MPI_Bcast(&columnas,1,  MPI_INT,  0, MPI_COMM_WORLD);

    A = new int *[filas];
    x = new int [columnas];

    if(my_rank==0){
    	A[0] = new int [filas*columnas];
        for ( int i = 1; i < filas; i++) {
            A[i] = A[i - 1] + columnas;
        }
        y = new int [filas];
        srand(time(0));
        cout << "La matriz generada " << endl;
        for (int i = 0; i < filas; i++) {
            for ( int j = 0; j < columnas; j++) {
                if (j == 0) cout << "[";
                A[i][j] = rand() % 10;
                cout << A[i][j];
                if (j == columnas - 1) cout << "]";
                else cout << "  ";
            }
            cout << "\n";
        }
        cout << "\nVector generado"<<endl;
        for (int i = 0; i < columnas; i++) {
        	 x[i] = rand() % 10;
        	cout << "[" << x[i] << "]" << endl;
        }


        // Reservamos espacio para la comprobacion
        comprueba = new int [filas];
        // Lo calculamos de forma secuencial
        for ( int i = 0; i < filas; i++) {
            comprueba[i] = 0;
            for ( int j = 0; j < columnas; j++) {
                comprueba[i] += A[i][j] * x[j];
            }
        }
    }


    // Reservamos espacio para la fila local de cada proceso
    miFila = new int [columnas];

    MPI_Scatter(A[0], columnas,MPI_INT, miFila, columnas, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(x, columnas, MPI_INT, 0, MPI_COMM_WORLD);


    MPI_Barrier(MPI_COMM_WORLD);
    // Inicio de medicion de tiempo
    tInicio = MPI_Wtime();

    long subFinal = 0;
    for ( int i = 0; i < columnas; i++) {
        subFinal += miFila[i] * x[i];
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // fin de medicion de tiempo
    tFin = MPI_Wtime();


    MPI_Gather(&subFinal, // Dato que envia cada proceso
            1, // Numero de elementos que se envian
            MPI_INT, // Tipo del dato que se envia
            y, // Vector en el que se recolectan los datos
            1, // Numero de datos que se esperan recibir por cada proceso
            MPI_INT, // Tipo del dato que se recibira
            0, // proceso que va a recibir los datos
            MPI_COMM_WORLD); // Canal de comunicacion (Comunicador Global)

    MPI_Finalize();

    if (my_rank == 0) {

        int errores = 0;

        cout << "El resultado obtenido y el esperado son:" << endl;
        for ( int i = 0; i < filas; i++) {
            cout << "\t" << y[i] << "\t|\t" << comprueba[i] << endl;
            if (comprueba[i] != y[i])
                errores++;
        }

        delete [] y;
        delete [] comprueba;
        delete [] A[0];

        if (errores) {
            cout << "Hubo " << errores << " errores." << endl;
        } else {
            cout << "No hubo errores" << endl;
            cout << "El tiempo tardado ha sido " << tFin - tInicio << " segundos." << endl;
        }
    }
    delete [] x;
    delete [] A;
    delete [] miFila;
}
