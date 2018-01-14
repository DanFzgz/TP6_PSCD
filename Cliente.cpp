//*****************************************************************
// File:   Cliente.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de cliente con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

int main(int argc,char *argv[]) {
	const string MENS_FIN("END OF SERVICE");
    // Dirección y número donde escucha el proceso servidor
    string SERVER_ADDRESS = argv[0];
    int SERVER_PORT = atoi(argv[1]);
	string url;
	if(argc==3){
		url=argv[2];
	}

	// Creación del socket con el que se llevará a cabo
	// la comunicación con el servidor.
	Socket socket(SERVER_ADDRESS, SERVER_PORT);
	srand(time(NULL));

    // Conectamos con el servidor. Probamos varias conexiones
	const int MAX_ATTEMPS = 10;
	int count = 0;
	int socket_fd;
	do {
		// Conexión con el servidor
    	socket_fd = socket.Connect();
    	count++;

    	// Si error --> esperamos 1 segundo para reconectar
    	if(socket_fd == -1){
    	    this_thread::sleep_for(chrono::seconds(1));
    	}
    } while(socket_fd == -1 && count < MAX_ATTEMPS);

    // Chequeamos si se ha realizado la conexión
    if(socket_fd == -1){
    	return socket_fd;
    }


    string mensaje;
	string buffer;
	bool terminar=false;
	int fila= rand()%4;
	int columna=rand()%10;

	do{
		socket.Recv(socket_fd,buffer,MESSAGE_SIZE);
		if(buffer.compare("Acaba subasta")==0){
			terminar=true;
		}
		else if(buffer.compare("URL que desearia mostrar: ")==0){
			if(argc==3){
				socket.Send(socket_fd,url);
			}
			else{
				cout << buffer ;
				cin >> mensaje;
				socket.Send(socket_fd,mensaje);
			}
		}
		else if(buffer.compare("Ha ganado la subasta")==0){
			cout << buffer <<endl;
		}
		else if(buffer.compare("Subasta finalizada, esperando a que comienze la siguiente... ")==0){
			cout << buffer <<endl;
		}
		else{
			cout << buffer <<"(y/n) : ";
			cin >> mensaje;
			while(mensaje.compare("n")!= 0 || mensaje.compare("y") != 0|| mensaje.size()==0){
				cout << "Comando no reconocido" << endl;
				cout << buffer <<"(y/n) : ";
				cin >> mensaje;
			}
			int send_bytes=socket.Send(socket_fd,mensaje);
			if(send_bytes == -1){
				cerr << "Error al enviar datos: " << strerror(errno) << endl;
				// Cerramos el socket
				socket.Close(socket_fd);
				exit(1);
			}
			else if(mensaje.compare("n")==0){
				cout << "Espero a otra subasta" <<endl;
			}
			else if(mensaje.compare("y")==0){
				cout << "Intento subir la puja" << endl;
			}
		}
	} while(!terminar);

	int error_code = socket.Close(socket_fd);
    if(error_code == -1){
		cerr << "Error cerrando el socket: " << strerror(errno) << endl;
    }

    return error_code;
}
