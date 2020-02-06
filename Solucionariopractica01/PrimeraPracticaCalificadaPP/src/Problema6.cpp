/*
 ============================================================================
 Name        : PrimeraPracticaCalificadaPP.c
 Author      : JoseAlberto
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
using namespace std;
int main (int argc, char * argv[]){
int size,rank;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD,&size);
MPI_Comm_rank(MPI_COMM_WORLD,&rank);

int *A,*local_a;
int tam,c,x,ntam,my_cont=0,our_cont;

if(rank==0){
	cout<<"Digite la cantidad de elementos del vector: ";
	cin>>tam;
	cout<<"Digite el valor a ubicar: ";
	cin>>x;
	if(tam<size){
		tam=size;
		cout<<"Orden del vector cambiado a: "<<tam<<endl;
	}else{
		if((tam%size) != 0){
			c=tam/size;
			tam=(c+1)*size;
			cout<<"Orden del vector cambiado a: "<<tam<<endl;
		}
	}
	ntam=tam/size;

}
MPI_Bcast(&tam,1,MPI_INT,0,MPI_COMM_WORLD);
MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);
MPI_Bcast(&ntam,1,MPI_INT,0,MPI_COMM_WORLD);
A= new int[tam];





local_a= new int [ntam];

if(rank==0){
	cout<<"Vector generado [ ";
	for(int i=0;i<tam;i++){
		A[i]=rand()%10;
		cout<<A[i]<<" ";
	}
	cout <<"]"<<endl;
	MPI_Scatter(A,ntam,MPI_INT,local_a,ntam,MPI_INT,0,MPI_COMM_WORLD);

}

MPI_Scatter(A,ntam,MPI_INT,local_a,ntam,MPI_INT,0,MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);
for(int i=0;i<ntam;i++){
	if(local_a[i]==x){
		my_cont++;
	}
}
MPI_Barrier(MPI_COMM_WORLD);

MPI_Reduce(&my_cont, &our_cont,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);


if(rank==0){
	cout<<"El numero de veces que se encontro "<<x<<" en el vector es: "<<our_cont<<endl;
}


MPI_Finalize();
delete [] A;
delete [] local_a;
return 0;
}
