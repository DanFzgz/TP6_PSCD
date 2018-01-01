//*****************************************************************
// File:	Valla.h
// Authors: Adrian Garcia Saez-Benito (NIP: 722292)
//			Jorge Fernandez Muñoz (NIP: 721529)
//			David Lacarta Anadon (NIP: 721511)
// Date:	Noviembre/Diciembre 2017
// Coms:	Monitor encargado de gestionar las Vallas del
//			trabajo practico de PSCD.
//*****************************************************************

#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>

#ifndef VALLA_H
#define VALLA_H

using namespace std;

const int numVallas = 2;

class Valla{
private:

	mutex mtx;
	condition_variable libre;
	condition_variable cola_espera;

	queue<string> cola;

	string laUrl;

	int laValla;
	int vallasLibres;

	bool valla[numVallas];

public:

	/*
	* Constructor.
	*/
	Valla();

	/*
	* Pre:	"url" es la URL de la imagen a mostrar en la Valla "valla" durante
	*		"tiempo" segundos.
	* Post:	Se ha mostrado la imagen contenida en la URL "url" en la Valla
	*		"valla" durante "tiempo" segundos.
	*/
	void mostrar(string url, int valla, int tiempo);

	/*
	* Pre:	Pide que la imagen contenida en la URL "url" sea encolada en el
	*		gestor para ser mostrada durante "tiempo" segundos.
	* Post:	La imagen contenida en la URL "url" ha sido encolada en el gestor
	*		para ser mostrada durante "tiempo" segundos.
	*/
	//void peticion(string url, int tiempo);


};

#endif
