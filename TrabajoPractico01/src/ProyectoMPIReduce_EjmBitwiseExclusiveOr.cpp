//============================================================================
// Name        : TrabajoPractico01.cpp
// Author      : JoseAlberto
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <math.h>
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;

int main(int argc, char *argv[])
{
    int tama, rank, size;
    vector<int> Arreglo, a;
    int parcial=111, bitwise; // Valor local de los elementos del arreglo



    MPI_Init(&argc, &argv); // Inicializamos los procesos
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero total de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Obtenemos el valor de nuestro identificador

    if (argc < 2) {
            if (rank == 0) {
                cout << "No se ha especificado numero de elementos, multiplo "
                		"de la cantidad de entrada, por defecto sera " << size * 100;
                cout << "\nUso: <ejecutable> <cantidad>" << endl;
            }
            tama = size * 100;
        } else {
            tama = atoi(argv[1]);
            if (tama < size)
            	tama = size;
            //Si la cantidad de elementos es menor a la de los procesos a utilizar
            //Entonces igualas la cantidad de procesos a la de elementos
            else {
                int i = 1, num = size;
                while (tama > num) {
                    ++i;
                    num = size*i;
                }
                if (tama != num) {
                    if (rank == 0)
                        cout << "Cantidad cambiada a " << num << endl;
                    tama = num;
                }
            //Si no se cumple, transformas el numero de elementos a un multiplo cercano de la
            // de la cantidad de procesos
            }
        }

    Arreglo.resize(tama, 0);
    a.resize(tama/size, 0);
    if(rank==0){
    	  srand(time(NULL)); //semilla aleatoria
    	    for(int i=0; i<tama; i++){
    	    	Arreglo[i]=100+rand()%(1000-1); // valores aleatorios del 1 al 100
    	    }


    }
    MPI_Scatter(&Arreglo[0], // Valores a compartir
               tama / size, // Cantidad que se envia a cada proceso
               MPI_INT, // Tipo del dato que se enviara
               &a[0], // Variable donde recibir los datos
               tama / size, // Cantidad que recibe cada proceso
               MPI_INT, // Tipo del dato que se recibira
               0,  // proceso principal que reparte los datos
               MPI_COMM_WORLD); // Comunicador (En este caso, el global)


   //Operaciones parciales de cada proceso
   for(int i=0; i< (tama/size);i++ ){
	   parcial = a[i] ^ parcial;
    }
	cout<<"Operacion Parcial: "<<parcial<<endl;


	MPI_Reduce(&parcial, // Valor local de PI
			   &bitwise,  // Dato sobre el que vamos a aplicar la operacion
			   1,	 // Numero de datos que vamos a reducir
			   MPI_INT,  // Tipo de dato que vamos a operar
			   MPI_BXOR,  // Operacion que aplicaremos
			   0, // proceso que va a recibir los datos operados
			   MPI_COMM_WORLD);

	// Solo el proceso 0 imprime el mensaje, ya que es la unica que
	// conoce el valor maximo
	if (rank == 0){

		cout << "El resultado de aplicar bitwise xor a los datos aleatorios es: " <<bitwise<<endl;
	}

	// Terminamos la ejecucion de los procesos, despues de esto solo existira
	// el proceso 0
    MPI_Finalize();
    return 0;
}

