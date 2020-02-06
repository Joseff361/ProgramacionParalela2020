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
int tam,c,ntam,my_f;
int my_factorial=1, our_factorial;
if(rank==0){
	cout<<"Digite el factorial a operar: ";
	cin>>tam;
	if(tam<size){
		tam=size;
		cout<<"Factorial cambiado a: "<<tam<<endl;
	}else{
		if((tam%size) != 0){
			c=tam/size;
			tam=(c+1)*size;
			cout<<"Factorial cambiado a: "<<tam<<endl;
		}
	}
	ntam=tam/size;
	//cout<<ntam<<endl;
}
MPI_Bcast(&tam,1,MPI_INT,0,MPI_COMM_WORLD);
MPI_Bcast(&ntam,1,MPI_INT,0,MPI_COMM_WORLD);

my_f=ntam*(rank+1);
for(int i=0; i<ntam;i++){
	my_factorial=my_factorial*(my_f-i);
}

MPI_Reduce(&my_factorial,&our_factorial,1,MPI_INT,MPI_PROD,0,MPI_COMM_WORLD);

if(rank==0){
	cout<<"El valor del factorial es: "<<our_factorial<<endl;
}



MPI_Finalize();
return 0;
}
