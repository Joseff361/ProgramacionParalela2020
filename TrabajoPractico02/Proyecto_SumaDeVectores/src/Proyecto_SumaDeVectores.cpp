/*
 ============================================================================
 Name        : Proyecto_SumaDeVectores.c
 Author      : JoseAlberto
 Version     :
 Copyright   : Your copyright notice
 Description : Compute Pi in MPI C++
 ============================================================================
 */
#include <math.h> 
#include "mpi.h" 
#include <iostream>
#include <stdlib.h>
using namespace std;
 
int main(int argc, char *argv[]) {
	int rank,size;
	double alocal[10], blocal[10],clocal[10];
	MPI::Init(argc, argv);
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();

	double x[100],y[100],z[100];
	for(int i=0;i<100;i++){
		x[i]=rand()%100;
		y[i]=rand()%100;
	}

	if(rank==0){
		for(int i=0; i<10;i++){
			MPI_Send(x+i*10,10,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
			MPI_Send(y+i*10,10,MPI_DOUBLE,i,1,MPI_COMM_WORLD);
			z[i]=x[i]+y[i];

		}
	}else{
			MPI_Recv(alocal,10,MPI_DOUBLE,0,0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(blocal,10,MPI_DOUBLE,0,1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//cout<<"Vector "<<rank<<endl;
			for(int i=0;i<10;i++){
				clocal[i]=alocal[i]+blocal[i];
				//cout<<clocal[i]<<" ";
			}
			//cout<<endl;
			MPI_Send(clocal,10,MPI_DOUBLE,0,rank+1,MPI_COMM_WORLD);
	}


	if(rank==0){
		for(int i=1; i<10;i++){
			MPI_Recv(z+i*10,10,MPI_DOUBLE,i,i+1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		}

		for(int i=0;i<100;i++){
			cout<<z[i]<<endl;
			//cout<<clocal[i]<<" ";
		}
	}




	MPI::Finalize();
	return 0;
}

