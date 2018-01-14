
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
	int numMensajes;
public:

	/*
	* Constructor
	*/
	Subasta();
	
	/*
	*
	* Pre: --
	*
	* Post: Si todavia no ha empezado la subasta, se aumenta el numero de participantes en 1, en caso contario, se aumenta en la siguiente ronda
	*
	*/
	void quieroParticipar();
	
	/*
	*
	* Pre:--
	*
	* Post: Se decrementa en 1 el numero de participantes, si hay una subasta activa, se espera a que acabe para informarlo
	*
	*/
	void quieroIrme();
	
	/*
	*
	* Pre: "cantidad" es el precio que ofrece el cliente "numSocket" en euros, y 0<=codigo<=2  
	*
	* Post: Si el codigo es 0, la puja tiene exito, si es 1 no y si es 2 significa que el cliente habia decidido abandonar la subasta
	*
	*/
	void participo(int cantidad, int numSocket,int codigo);
	
	/*
	*
	* Pre:--
	*
	* Post: Dan comienzo las subastas asi que, el numero de clientes es 3 o mas y el numero de urls es 1
	*
	*/
	void iniciarSubasta();
	
	/*
	*
	* Pre:--
	*
	* Post: Pone fin a las subastas asi que, el numero de clientes que han dicho que no es igual al numero de clientes menos 1 y el numero de urls es distinto de 1
	*
	*/
	void finalizarSubastas();
	
	/*
	*
	* Pre:--
	*
	* Post: Da comienzo una nueva ronda sin subastas
	*
	*/
	void iniciarRonda();
	
	/*
	*
	* Pre:--
	*
	* Post: Finaliza la ronda actual por lo que el numero de subastas es igual al numero de clientes
	*
	*/
	void finalizarRonda();
	
	/*
	*
	* Pre:--
	*
	* Post: Devuelve true si y solo si finaliza la subasta
	*
	*/
	bool acabaSubasta();
	
	/*
	*
	* Pre:--
	*
	* Post: Devuelve, si hay una subasta activa, el proximo precio de esta
	*
	*/
	int proximoPrecio();
	
	/*
	*
	* Pre:--
	*
	* Post: Devuelve verdad si y solo si existen ganadores
	*
	*/
	bool hayGanadores();
	
	/*
	*
	* Pre:--
	*
	* Post: Devuelve el identificador del cliente ganador
	*
	*/
	int ganador();
	
	/*
	*
	* Pre:--
	*
	* Post: Devuelve el precio maximo ofrecido en la subasta
	*
	*/
	int precio();
	
	/*
	*
	* Pre:--
	*
	* Post: aumenta en 1 el numero de urls
	*
	*/
	void mensaje();
	
	/*
	*
	* Pre:--
	*
	* Post: Pone a 0 el numero de urls
	*
	*/
	void esperarMensaje();
	
	/*
	*
	* Pre:--
	*
	* Post: Espera hasta que llegue la url
	*
	*/
	void llegaMensaje();
};

#endif
