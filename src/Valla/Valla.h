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
#include <iostream>
#include <condition_variable>
#include <queue>
#include <string>
#include <thread>
#include "ImageDownloader.hpp"
#include "CImg.h"

#ifndef VALLA_H
#define VALLA_H

using namespace std;

const int numVallas = 2;

class Valla{
private:

	mutex mtx;
	condition_variable cv, termina;

	queue<string> cola;

	bool finish;

	int numIm; //numero de imagenes mostradas
	double tiempoTotal; // tiempo total de imagenes mostrandose
	int petEncoladas; // numero de peticiones encoladas
	double tiempoEstimado; // tiempo contratado estimado
	int contador;


public:

	Valla();

	void mostrar(string& Url);

    void terminar();

    void avisar(string url);

	void notificar();

	void informar();

	void iniciarValla();

	void terminarValla();
};

#endif
