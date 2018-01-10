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
#include "ImageDownloader.hpp"

#ifndef VALLA_H
#define VALLA_H

using namespace std;

const int numVallas = 2;
const int VALLA_WIDTH = 800;
const int VALLA_HEIGHT = 800;

class Valla{
private:

	ImageDownloader downloader;

	mutex mtx;
	condition_variable libre;
	condition_variable cola_espera;

	queue<string> cola;

	string laUrl;


	int laValla;
	int vallasLibres;

	bool vallas[numVallas];

	/*
	* Pre:	"url" contiene la URL donde está alojada la imagen a mostrar, "tiempo"
	*		es el tiempo que se va a mostrar y "valla" es el numero de valla en
	*		la que se va a mostrar.
	* Post:	La imagen contenida en la URL "url" ha sido mostrada en la Valla
	*		numero "valla" durante "tiempo" segundos.
	*/
	void mostrar(string url, int valla, int tiempo);

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
	void atender(string url, int tiempo);

};

#endif
