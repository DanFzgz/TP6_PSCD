#ifndef SUBASTA_H
#define SUBASTA_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <fstream>

using namespace std;

class Subasta {
private:

	mutex mtx;

	condition_variable subasta; // Cada ronda cada cliente elije si sube el precio o no, y despues hace wait hasta la siguiente ronda sobre esta condicion.
	condition_variable esperaSubasta; // El subastador espera a que haya como minimo 3 participantes para comenzar la subasta.

	int numClientes; //numero de clientes que actualmente estan en la subasta.
	int numNos; //numero de participantes que no quieren subir la apuesta.
	int numSubastas; //Indica cuantos clientes ya han realizado su apuesta en esta ronda.
	int numMensajes;
	int FondoReserva; //Indica el minimo precio que pide el subastador
	int maximoOfrecido; // indica la cantidad maxima ofrecida.
	int clienteGanador; // indica el socket del cliente ganador
	int cantidad; //Precio al que va la subasta

	bool enSubasta; //Indica si hay una Subasta en curso.
	bool terminacion; //Indica si el administrador quiere finalizar el programa.

public:
	Subasta();
	void quieroParticipar();
	void salir();
	void participo(int cantidad, int numSocket,int codigo);
	void iniciarSubasta();
	void finalizarSubastas();
	void iniciarRonda();
	void finalizarRonda();
	bool acabaSubasta();
	int proximoPrecio();
	bool hayGanadores();
	int ganador();
	int precio();
	void mensaje();
	void esperarMensaje();
	void llegaMensaje();
};

#endif
