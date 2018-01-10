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
#include "subasta.hpp"

using namespace std;

Subasta s;

//-------------------------------------------------------------
void administrador(Socket&soc,int numSocket) {
	cout << "Soy el administrador" <<endl;
	string orden;
	cin>> orden;
	while(orden.compare("Finalizar")!=0){
		cin >>orden;
	}
	s.finalizarSubastas();
	cout <<"Me voy a morir"<<endl;

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
		}
		else{
			cout <<"Subasta finalizada con precio inferior al minimo" <<endl;
		}
		s.iniciarSubasta();
	}
	cout <<"Subastas finalizadas" <<endl;
}
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd) {
	int MAX_BUFFER=50;
	s.quieroParticipar();
	cout <<" nuevo participante apuntado" <<endl;
	string buffer;
	bool meVoy=false;
	while(!s.acabaSubasta()&& !meVoy ){
		while(!s.hayGanadores()&& !meVoy){
			cout<< "voy a preguntar" <<endl;
			int precio= s.proximoPrecio();
			string mensaje="Proxima apuesta "+ to_string(precio);
			int send_bytes= soc.Send(client_fd,mensaje);
			if(send_bytes == -1) {
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos los sockets
				soc.Close(client_fd);
				exit(1);
			}
			soc.Recv(client_fd,buffer,MAX_BUFFER);
			if(buffer.compare("retirar")==0){
				s.participo(precio,client_fd,2);
				s.quieroIrme();
				meVoy=true;
				soc.Close(client_fd);
			}
			else if(buffer.compare("n")==0){
				cout <<"paso" <<endl;
				s.participo(precio,client_fd,1);
			}
			else{
				cout <<"modifico y espero" <<endl;
				s.participo(precio,client_fd,0);
			}
		}
	}
	buffer="Acaba subasta";
	soc.Send(client_fd,buffer);
	soc.Close(client_fd);
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
	subasta= thread(&subastador);
	int i=0;
	while(i<max_connections && !s.acabaSubasta()) {
		// Accept
		client_fd[i] = socket.Accept();

		if(client_fd[i] == -1) {
			cerr << "Error en el accept: " << strerror(errno) << endl;
			// Cerramos el socket
			socket.Close(socket_fd);
			break;
		}
		cout << "nuevoCliente aceptado" <<endl;
		thread t = thread(&servCliente, ref(socket), client_fd[i]);
		t.detach();
		i++;
	}
    return 0;
}
//-------------------------------------------------------------
