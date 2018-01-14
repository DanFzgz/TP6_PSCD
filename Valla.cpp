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
	numIm=0;
	tiempoTotal=0;
	petEncoladas=0;
	tiempoEstimado=0;
	finish = false;
}


void Valla::mostrar(string& url){

	unique_lock<mutex> lck(mtx);

	if(finish && cola.empty()){
		url = "";
	}
	else{
		while(cola.empty() && !finish){
			cv.wait(lck);
		}
		if(finish && cola.empty()){
			url="";
		}
		else{
			url = cola.front();
			cola.pop();
			--petEncoladas;
			++numIm;
			tiempoTotal+=60;
		}
	}
}

void Valla::terminar(){
	unique_lock<mutex> lck(mtx);
	finish = true;
	while(contador > 0){
		termina.wait(lck);
	}
}

void Valla::notificar(){
	unique_lock<mutex> lck(mtx);

	cv.notify_all();
}

void Valla::avisar(string url){

	unique_lock<mutex> lck(mtx);
	cola.push(url);
    cv.notify_one();
	petEncoladas++;

}

void Valla::informar(){
	unique_lock<mutex> lck(mtx);
	cout << "INFORMACION HISTORICA DEL SISTEMA\n"
		 << "#################################"
		 << "Numero de imagenes mostradas: " << numIm << "\n"
		 << "Tiempo total que las imagenes han estado en pantalla: " << tiempoTotal << "\n"
		 << "Tiempo medio que las imagenes han estado en pantalla: " << tiempoTotal/numIm << "\n"
		 << "INFORMACION DEL ESTADO DEL SISTEMA" << "\n"
		 << "##################################"
		 << "Numero de peticiones encoladas: " << petEncoladas << "\n"
		 << "Tiempo contratado estimado: " << tiempoEstimado << endl;
}


void Valla::iniciarValla(){
	unique_lock<mutex> lck(mtx);
	contador++;
}

void Valla::terminarValla(){
	unique_lock<mutex> lck(mtx);
	contador--;
	termina.notify_all();
}
