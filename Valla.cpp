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
	finish = false;
	tiempo = 30;
}

void Valla::mostrar1(string url){

    vallas[0]=false;
    --vallasLibres;

	char URL[500];
	for(int x=0; x<url.length();++x){
		URL[x] = url[x];
	}
	char path[100] = "1.jpg";
	char nombreValla[10] = "Valla 1";

	// Creamos una valla publicitaria con una imagen

	downloader.downloadImage(URL, path);
	cimg_library::CImg<unsigned char> img_principal(path);
	cimg_library::CImgDisplay valla1(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), nombreValla);
	valla1.resize(VALLA_WIDTH,VALLA_HEIGHT);
	valla1.move(0, 0);

	// Mostrar imagen durante tiempo segundos
	valla1.wait(tiempo*1000);

    vallas[0]=true;
    ++vallasLibres;

    cv.notify_all();

}

void Valla::mostrar2(string url){

    vallas[1]=false;
    --vallasLibres;

    char URL[500];
    for(int x=0; x<url.length();++x){
        URL[x] = url[x];
    }
    char path[100] = "2.jpg";
    char nombreValla[10] = "Valla 2";

    // Creamos una valla publicitaria con una imagen

    downloader.downloadImage(URL, path);
    cimg_library::CImg<unsigned char> img_principal(path);
    cimg_library::CImgDisplay valla2(img_principal.resize(VALLA_WIDTH,VALLA_HEIGHT), nombreValla);
    valla2.resize(VALLA_WIDTH,VALLA_HEIGHT);
    valla2.move(VALLA_WIDTH, 0);

    // Mostrar imagen durante tiempo segundos
    valla2.wait(tiempo*1000);

    vallas[1]=true;
    ++vallasLibres;

    cv.notify_all();

}

void Valla::fin(){

}

void Valla::terminar(){
	finish = true;
}

bool Valla::ended(){
    return finish;
}

bool Valla::libre1(){
    return vallas[0];
}

bool Valla::libre2(){
    return vallas[1];
}

void Valla::sumarImagenes(){
	unique_lock<mutex> lck(mtx);
	numIm++;
}

void Valla::sumarTiempo(double t){
	unique_lock<mutex> lck(mtx);
	tiempoTotal=tiempoTotal+t;
}

void Valla::sumarPeticion(){
	unique_lock<mutex> lck(mtx);
	petEncoladas++;
}

void Valla::restarPeticion(){
	unique_lock<mutex> lck(mtx);
	petEncoladas--;
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
