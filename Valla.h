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
const int VALLA_WIDTH = 800;
const int VALLA_HEIGHT = 800;

class Valla{
private:

	ImageDownloader downloader;

	mutex mtx;
	condition_variable cv;

	int laValla;
	int vallasLibres;
	int tiempo;

	bool finish;
	bool vallas[numVallas];

	int numIm; //numero de imagenes mostradas
	double tiempoTotal; // tiempo total de imagenes mostrandose
	int petEncoladas; // numero de peticiones encoladas
	double tiempoEstimado; // tiempo contratado estimado


public:

	Valla();

    void mostrar1(string url);

    void mostrar2(string url);

	void mostrar(queue<string>& cola);

    void fin();

    void terminar();

    bool ended();

    bool libre1();

    bool libre2();

	/*
	* Pre: --
	*
	* Post: Se incrementa en 1 el numero de imagenes mostradas
	*/
	void sumarImagenes();

	/*
	* Pre: "t" es un tiempo en segundos
	*
	* Post: Se incrementa en t el tiempo total que las imagenes han estado en pantalla
	*/
	void sumarTiempo(double t);

	/*
	* Pre: --
	*
	* Post: Se incrementa en 1 el numero de peticiones encoladas
	*/
	void sumarPeticion();

	/*
	* Pre: --
	*
	* Post: Se decrementa en 1 el numero de peticiones encoladas
	*/
	void restarPeticion();

	void informar();
};

#endif
