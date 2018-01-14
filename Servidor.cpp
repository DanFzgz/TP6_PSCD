//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante "make".
//*****************************************************************

#include "Socket.hpp"
#include <iostream>
#include <thread>
#include <cstring> //manejo de cadenas tipo C
#include <cstdlib>
#include <chrono>
#include "Subasta.hpp"
#include "Valla.h"

using namespace std;

Subasta s;
Valla v;
queue<string> cola_url;
queue<int> cola_tiempo;

//-------------------------------------------------------------
void administrador(Socket&soc,int numSocket) {
	cout << "Soy el administrador" << endl;
	string orden;
	cin>> orden;
	while(orden.compare("Finalizar")!=0){
		if(orden.compare("Informar")==0){
			v.informar();
		}
		cin >>orden;
	}
	s.finalizarSubastas();
	s.llegaMensaje();


    // Cerramos el socket del servidor
    int error_code = soc.Close(numSocket);
    if(error_code == -1) {
		cerr << "Error cerrando el socket del servidor: " << strerror(errno) << endl;
	}

	// Mensaje de despedida
	cout << "Bye bye" << endl;
	exit(0);
}

//-------------------------------------------------------------

void subastador(){
	cout << "La proxima subasta comenzara en 5 segundos " << endl;
	this_thread::sleep_for(chrono::seconds(5);
	s.iniciarSubasta();
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
			cout << "La proxima subasta comenzara en 5 segundos " << endl;
			this_thread::sleep_for(chrono::seconds(5);
			s.iniciarSubasta();
		}
		else{
			cout <<"Subasta finalizada con precio inferior al minimo" <<endl;
			cout << "La proxima subasta comenzara en 5 segundos " << endl;
		this_thread::sleep_for(chrono::seconds(5);
			s.iniciarSubasta();
		}

	}
	cout <<"Subastas finalizadas" <<endl;
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
			cola_url.push(url);
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

void atender(){
	string url;
	while(!v.ended()){
		v.mostrar(cola_url);
	}
}
//-------------------------------------------------------------
int main(int argc,char *argv[]) {
	const int N = 5;
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
	valla1 = thread(&atender);
	valla2 = thread(&atender);
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
    return 0;
}
//-------------------------------------------------------------
