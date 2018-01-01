//*****************************************************************
// File:	Valla.cpp
// Authors: Adrian Garcia Saez-Benito (NIP: 722292)
//			Jorge Fernandez Muñoz (NIP: 721529)
//			David Lacarta Anadon (NIP: 721511)
// Date:	Noviembre/Diciembre 2017
// Coms:	Monitor encargado de gestionar las Vallas del
//			trabajo practico de PSCD.
//*****************************************************************

#include "Valla.h"

Valla::Valla(){
	vallasLibres = 2;
	for(int x = 0; x<numVallas; ++x){ valla[x] = true; }
}

void Valla::mostrar(string url, int valla, int tiempo){

	unique_lock<mutex> lck(mtx);

	if (vallasLibres == 0){ cola.push(url); }

	laUrl = cola.front();
	if(!cola.empty()){
		while(vallasLibres == 0){
			while(laUrl != url){
				cola_espera.wait(lck);
				laUrl = cola.front();
			}
		}
	}
}
