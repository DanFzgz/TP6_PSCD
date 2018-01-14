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
	numIm=0;
	tiempoTotal=0;
	petEncoladas=0;
	tiempoEstimado=0;
	for(int x = 0; x<numVallas; ++x){ vallas[x] = true; }
}

void Valla::atender(string url, int tiempo){

	unique_lock<mutex> lck(mtx);

	if (vallasLibres == 0 || !cola.empty()){
		cola.push(url);
		petEncoladas++;
		laUrl = cola.front();
		while(laUrl != url){
			cola_espera.wait(lck);
			laUrl = cola.front();
		}
	}



	while(vallasLibres == 0){
		libre.wait(lck);
	}

	for(int laValla = 0; laValla < numVallas; ++laValla){
		if(vallas[laValla]){
			--vallasLibres;
			vallas[laValla] = false;

			mostrar(url, laValla+1, tiempo);
			numIm++;
			tiempototal=tiempoTotal+tiempo;
		}
	}
	cola.pop();
	petEncoladas--;
	finaliza.notify_one();
	cola_espera.notify_all();

}
void Valla::mostrar(string url, int numValla, int tiempo){

	char URL[500];
	for(int x=0; x<url.length();++x){
		URL[x] = url[x];
	}
	char path[100] = " .jpg";
	char nombreValla[10] = "Valla  ";
	int x = 0, y = 0;

	// Creamos una valla publicitaria con una imagen

	if(numValla == 1){
		path[0] = 1 + '0';
		nombreValla[6] = 1 + '0';
	}
	else if(numValla == 2) {
		path[0] = 2 + '0';
		nombreValla[6] = 2 + '0';
		x = VALLA_WIDTH;
	}

	downloader.downloadImage(URL, path);
	cimg_library::CImg<unsigned char> img_principal(path);
	cimg_library::CImgDisplay valla(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), nombreValla);
	valla.resize(VALLA_WIDTH,VALLA_HEIGHT);
	valla.move(x, y);


	// Mostrar imagen durante tiempo segundos
	valla.wait(tiempo*1000);
}

void Valla::fin(){

	unique_lock<mutex> lck(mtx);

	while(!cola.empty() && vallasLibres == 2){
		finaliza.wait(lck);
	}
}

void Subasta::sumarImagenes(){
	unique_lock<mutex> lck(mut);
	numIm++;
}

void Subasta::sumarTiemo(double t){
	unique_lock<mutex> lck(mut);
	tiempoTotal=tiempoTotal+t;
}

void Subasta::sumarPeticion(){
	unique_lock<mutex> lck(mut);
	petEncoladas++;
}

void Subasta::restarPeticion(){
	unique_lock<mutex> lck(mut);
	petEncoladas--;
}



void Subasta::informar(){
	unique_lock<mutex> lck(mut);
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
