
#include "Subasta.hpp"


Subasta::Subasta(){

	numClientes = 0;
	numNos = -1;
	numSubastas = 0;
	clienteGanador = -1;
	cantidad = 60;
	numMensajes = 1;
	enSubasta = false;
	terminacion = false;

}


void Subasta::quieroParticipar(){

	unique_lock<mutex> lck(mtx);

	while(enSubasta){   //Si esta realizandose una ronda, espera a que acabe y se apunta a la siguiente ronda
		subasta.wait(lck);
	}

	numClientes++;
	cout << "Nuevo participante apuntado" << endl;
	esperaSubasta.notify_all();

}

void Subasta::salir(){

	unique_lock<mutex> lck(mtx);

	while(enSubasta){  //Si hay una ronda en juego, espera a que finalize para comunicar que no va a seguir.
		subasta.wait(lck);
	}

	numClientes--;

}


void Subasta::participo(int cantidad, int numSocket,int codigo){

	unique_lock<mutex> lck(mtx);

	if(cantidad>maximoOfrecido && codigo==0){
		cout << "El cliente " << numSocket << " ha decidido subir la cantidad a " << cantidad << endl;
		maximoOfrecido = cantidad;
		clienteGanador = numSocket;
	}
	if(codigo!=2){
		numSubastas++;
	}
	if(codigo==1){
		cout << "El cliente " << numSocket << " ha decidido no pujar esta vez" << endl;
		numNos++;
	}
	if(codigo==2){
		cout << "El cliente ha decidido abandonar la subasta" << endl;
		numClientes--;
	}
	if(numSubastas == numClientes){
		enSubasta = false;
		subasta.notify_all();
	}
	else{
		subasta.wait(lck);
	}

}

void Subasta::iniciarSubasta(){

	unique_lock<mutex> lck(mtx);

	while(numClientes<3 || numMensajes!=1){
		esperaSubasta.wait(lck);
	}

	FondoReserva = 70;
	numNos = -1;
	enSubasta = false;
	maximoOfrecido = 0;
	clienteGanador = -1;
	cantidad = 60;

	subasta.notify_all();

}

void Subasta::finalizarSubastas(){

	unique_lock<mutex> lck(mtx);

	while(numNos!=numClientes-1||numMensajes==1){
		subasta.wait(lck);
	}

	enSubasta=false;
	terminacion=true;
	cout << "Me voy a morir" << endl;
	subasta.notify_all();

}

void Subasta::iniciarRonda(){

	unique_lock<mutex> lck(mtx);

	enSubasta=true;
	numSubastas=0;
	numNos=0;
	subasta.notify_all();

}

void Subasta::finalizarRonda(){

	unique_lock<mutex> lck(mtx);

	while(numSubastas!=numClientes){
		subasta.wait(lck);
	}
	enSubasta=false;
	if(numNos<numClientes-1){
		cantidad=cantidad+10;
	}
	subasta.notify_all();

}

bool Subasta::acabaSubasta(){

	unique_lock<mutex> lck(mtx);

	while(numMensajes==0){
		subasta.wait(lck);
	}
	return terminacion;

}

int Subasta::proximoPrecio(){

	unique_lock<mutex> lck(mtx);

	while(!enSubasta){
		subasta.wait(lck);
	}
	cout<< "voy a preguntar" <<endl;
	return cantidad;

}

bool Subasta::hayGanadores(){

	unique_lock<mutex> lck(mtx);

	while(enSubasta){
		subasta.wait(lck);
	}
	return numNos== numClientes-1;

}

int Subasta::ganador(){

	unique_lock<mutex> lck(mtx);

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

	unique_lock<mutex> lck(mtx);

	while(enSubasta){
		subasta.wait(lck);
	}
	return maximoOfrecido;

}

void Subasta::mensaje(){

	unique_lock<mutex> lck(mtx);

	numMensajes++;

	esperaSubasta.notify_all();

}

void Subasta::esperarMensaje(){

	unique_lock<mutex> lck(mtx);

	numMensajes=0;
	subasta.notify_all();

}

void Subasta::llegaMensaje(){

	unique_lock<mutex> lck(mtx);

	while(numMensajes==0){
		subasta.wait(lck);
	}

}
