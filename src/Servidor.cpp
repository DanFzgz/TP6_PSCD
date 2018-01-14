//*****************************************************************
// File:	Servidor.cpp
// Authors: Adrian Garcia Saez-Benito (NIP: 722292)
//			Jorge Fernandez Muñoz (NIP: 721529)
//			David Lacarta Anadon (NIP: 721511)
// Date:	Noviembre/Diciembre 2017
// Coms:	Servidor del TP6 de PSCD
//*****************************************************************

#include "Socket/Socket.hpp"
#include <iostream>
#include <thread>
#include <cstring> //manejo de cadenas tipo C
#include <cstdlib>
#include <chrono>
#include "Subasta/Subasta.hpp"
#include "Valla/Valla.h"


using namespace std;

Subasta s;
Valla v;

ImageDownloader downloader;


int tiempo = 10;
const int VALLA_WIDTH = 800;
const int VALLA_HEIGHT = 800;

//-------------------------------------------------------------
void administrador(Socket&soc,int numSocket) {
	cout << "Soy el administrador\n";
	string orden;
	cin>> orden;
	while(orden.compare("Finalizar")!=0){
		if(orden.compare("Informar")==0){
			v.informar();
		}
		cin >>orden;
	}
	s.finalizarSubastas();
	// Cerramos el socket del servidor
	int error_code = soc.Close(numSocket);
	if(error_code == -1) {
		cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
	}
	v.terminar();
	s.llegaMensaje();

	// Mensaje de despedida
	cout << "Bye bye" << endl;
}

//-------------------------------------------------------------

void subastador(){
	s.iniciarSubasta();
	cout << "Minimo de clientes alcanzado, la proxima subasta comenzara en 5 segundos\n";
	this_thread::sleep_for(chrono::seconds(5));
	cout <<"Empieza la subasta" <<endl;
	while(!s.acabaSubasta()){
		while(!s.hayGanadores()){
			cout <<"inicio ronda" <<endl;
			s.iniciarRonda();
			s.finalizarRonda();
			cout <<"Acabo ronda" <<endl;
		}

		int ganador=s.ganador();
		int precio= s.precio();
		if(ganador>0){
			cout<< "El concursante" << ganador <<"ha ganado la subasta con " <<precio <<endl;
			s.esperarMensaje();
			s.iniciarSubasta();
			cout << "Minimo de clientes alcanzado, la proxima subasta comenzara en 5 segundos\n";
			this_thread::sleep_for(chrono::seconds(5));
		}
		else{
			cout <<"Subasta finalizada con precio inferior al minimo\n";
			s.iniciarSubasta();
			cout << "Minimo de clientes alcanzado, la proxima subasta comenzara en 5 segundos\n";
			this_thread::sleep_for(chrono::seconds(5));
		}

	}
	cout <<"Subastas finalizadas" <<endl;
	v.notificar();
}
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd) {
	int MAX_BUFFER=200;
	s.quieroParticipar();
	string mensaje;
	string buffer;
	string url;
	int resul;
	bool meVoy=false;
	while(!s.acabaSubasta()&& !meVoy ){
		while(!s.hayGanadores()&& !meVoy){
			int precio= s.proximoPrecio();
			mensaje="Proxima apuesta "+ to_string(precio);
			int send_bytes= soc.Send(client_fd,mensaje);
			if(send_bytes == -1) {
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos los sockets
				s.participo(precio,client_fd,2);
				meVoy=true;
				soc.Close(client_fd);
			}
			int recv_bytes=soc.Recv(client_fd,buffer,MAX_BUFFER);
			if(recv_bytes== -1){
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				s.participo(precio,client_fd,2);
				meVoy=true;
				soc.Close(client_fd);
			}
			if(buffer.size()==0){// reconoce el control c
				cout <<"Control C pillado" <<endl;
				s.participo(precio,client_fd,2);
				meVoy=true;
			}
			else if(buffer.compare("n")==0){
				s.participo(precio,client_fd,1);
			}
			else if(buffer.compare("y")==0){
				s.participo(precio,client_fd,0);
			}
		}
		if(s.ganador()==client_fd&&!meVoy){
			mensaje="Ha ganado la subasta";
			soc.Send(client_fd,mensaje);
			mensaje="URL que desearia mostrar: ";
			soc.Send(client_fd,mensaje);
			soc.Recv(client_fd,url,MAX_BUFFER);
			v.avisar(url);
			//cola.notify_all();
			s.mensaje();
			cout <<"URL: " << url <<endl;
		}
		else if(s.ganador()==client_fd && meVoy){
			cout << "El cliente ganador ha abandonado la subasta" <<endl;
			s.mensaje();
		}
		else if(s.ganador()!=client_fd&& !meVoy){
			string mensaje="Subasta finalizada, esperando a que comienze la siguiente... ";
			soc.Send(client_fd,mensaje);
		}
	}
	if(!meVoy){
		buffer="Acaba subasta";
		soc.Send(client_fd,buffer);
		soc.Close(client_fd);
	}
}

void mostrar1(string url){
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
}

void mostrar2(string url){
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
}

void atender(int id_valla){
	v.iniciarValla();
	cout << "Inicio thread\n";
	string url;
	v.mostrar(url);
	while(!url.empty()){
		cout << "Voy a mostrar\n";
		if(id_valla==1){
			mostrar1(url);
		}
		else if(id_valla==2){
			mostrar2(url);
		}
		v.mostrar(url);
	}
	cout << "Termino thread\n";
	v.terminarValla();
}
//-------------------------------------------------------------
int main(int argc,char *argv[]) {
	const int N = 10;
	// Dirección y número donde escucha el proceso servidor
	string SERVER_ADDRESS = "localhost";
    int SERVER_PORT = atoi(argv[1]);
    thread cliente[N];
	thread subasta;
	thread admin;
	thread valla1;
	thread valla2;
    int client_fd[N];

		//srand(time(NULL));

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_PORT);

	// Bind
	int socket_fd =socket.Bind();
	if (socket_fd == -1) {
		cerr << "Error en el bind: " << strerror(errno) << endl;
		exit(1);
	}

	// Listen
    int max_connections = N;
	int error_code = socket.Listen(max_connections);
	if(error_code == -1) {
		cerr << "Error en el listen: " << strerror(errno) << endl;
		// Cerramos el socket
		socket.Close(socket_fd);
		exit(1);
	}

	admin = thread(&administrador,ref(socket),socket_fd);
	subasta = thread(&subastador);
	valla1 = thread(&atender, 1);
	valla2 = thread(&atender, 2);
	int i=0;
	while(i<max_connections && !s.acabaSubasta()) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			//cerr << "Error en el accept: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			break;
		}
		cout << "Nuevo cliente aceptado" <<endl;
		thread t = thread(&servCliente, ref(socket), client_fd[i]);
		t.detach();
		i++;
	}
	subasta.join();
	admin.join();
	valla1.join();
	valla2.join();
    return 0;
}
//-------------------------------------------------------------
