#ifndef SUBASTA_H
#define SUBASTA_H

#include <mutex>
#include <condition_variable>
#include <assert.h>
#include <queue>

using namespace std;

class Subasta {
private:
	mutex mut;
	int numClientes; //numero de clientes que actualmente estan en la subasta.
	int numNos; //numero de participantes que no quieren subir la apuesta.
	int numSubastas; //Indica cuantos clientes ya han realizado su apuesta en esta ronda.
	int FondoReserva; //Indica el minimo precio que pide el subastador
	int maximoOfrecido; // indica la cantidad maxima ofrecida.
	int clienteGanador; // indica el socket del cliente ganador
	int cantidad; //Precio al que va la subasta
	condition_variable subasta; // Cada ronda cada cliente elije si sube el precio o no, y despues hace wait hasta la siguiente ronda sobre esta condicion.
	condition_variable esperaSubasta; // El subastador espera a que haya como minimo 3 participantes para comenzar la subasta.
	bool enSubasta; //Indica si hay una ronda en proceso.
	bool terminacion; //Indica si el administrados quiere finalizar el programa.
public:
	Subasta();
	void quieroParticipar();
	void quieroIrme();
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
};
	
#endif
	