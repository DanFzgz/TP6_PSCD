#include <iostream>
#include <fstream>
#include <queue>
#include <subasta.hpp>


Subasta::Subasta(){
	numClientes=0;
	numNos=0;
	numSubastas=0;
	clienteGanador=-1;
	cantidad=60;
	enSubasta=false;
	terminacion=false;
}


void Subasta::quieroParticipar(){
	unique_lock<mutex> lck(mut);
	while(enSubasta){   //Si esta realizandose una ronda, espera a que acabe y se apunta a la siguiente ronda
		subasta.wait(lck);
	}
	numClientes++;
	esperaSubasta.notify_all();
}

void Subasta::quieroIrme(){
	unique_lock<mutex> lck(mut);
	while(enSubasta){  //Si hay una ronda en juego, espera a que finalize para comunicar que no va a seguir.
		subasta.wait(lck);
	}
	numClientes--;
}


void Subasta::participo(int cantidad, int numSocket,int codigo){
	unique_lock<mutex> lck(mut);
	if(cantidad> maximoOfrecido&& codigo==0){
		maximoOfrecido=cantidad;
		clienteGanador=numSocket;
	}
	numSubastas++;
	if(codigo==1){
		numNos++;
	}
	if(numSubastas== numClientes){
		enSubasta=false;
		subasta.notify_all();
	}
	else{
		subasta.wait(lck);
	}
}

void Subasta::iniciarSubasta(){
	unique_lock<mutex> lck(mut);
	while(numClientes<3){
		esperaSubasta.wait(lck);
	}
	FondoReserva=70;
	numNos= 0;
	enSubasta=false;
	maximoOfrecido=0;
	clienteGanador=-1;
	cantidad=60;
	subasta.notify_all();
}

void Subasta::finalizarSubastas(){
	unique_lock<mutex> lck(mut);
	while(numNos!=numClientes-1){
		subasta.wait(lck);
	}
	enSubasta=false;
	terminacion=true;
	subasta.notify_all();
}

void Subasta::iniciarRonda(){
	unique_lock<mutex> lck(mut);
	enSubasta=true;
	numSubastas=0;
	numNos=0;
	subasta.notify_all();
}

void Subasta::finalizarRonda(){
	unique_lock<mutex> lck(mut);
	while(numSubastas!=numClientes){
		subasta.wait(lck);
	}
	enSubasta=false;
	if(numNos!=numClientes-1){
		cantidad=cantidad+10;
	}
	subasta.notify_all();
}

bool Subasta::acabaSubasta(){
	unique_lock<mutex> lck(mut);
	return terminacion;
}

int Subasta::proximoPrecio(){
	unique_lock<mutex> lck(mut);
	while(!enSubasta){
		subasta.wait(lck);
	}
	return cantidad;
}

bool Subasta::hayGanadores(){
	unique_lock<mutex> lck(mut);
	while(enSubasta){
		subasta.wait(lck);
	}
	return numNos== numClientes-1;
}

int Subasta::ganador(){
	unique_lock<mutex> lck(mut);
	while(enSubasta){
		subasta.wait(lck);
	}
	if(maximoOfrecido>FondoReserva){
		return clienteGanador;
	}
	else{
		return 0;
	}
}

int Subasta::precio(){
	unique_lock<mutex> lck(mut);
	while(enSubasta){
		subasta.wait(lck);
	}
	return maximoOfrecido;
}
	