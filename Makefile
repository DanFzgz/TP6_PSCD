#*****************************************************************
# File:   Makefile
# Author: PSCD-Unizar
# Date:   noviembre 2015
# Coms:   Genera sockets, cliente y servidor para la práctica 5
#         Ejecutar como "make"
#         Para compilar en Hendrix (Solaris OS), descomentar
#         "#${SOCKETSFLAGS}"
#*****************************************************************

# ######## #
# COMANDOS #
# ######## #
RM = rm -f # Comando de limpieza
CC = g++ # Comando de compilacion

# ######################## #
# VARIABLES GLOBALES       #
# Definimos:               #
#   * Librería de sockets  #
#   * Cliente              #
#   * Servidor             #
#   * ServidorMulticliente #
# ######################## #
SOCKET=Socket
CLIENTE=Cliente
SERVER=Servidor
VALLA=Valla
SUBASTA=Subasta
IMDOWNLOADER=ImageDownloader

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I. -I/usr/local/include -I/opt/X11/include -O2 -std=c++11  # -lsockets Flags compilacion
LDFLAGS=-L/usr/X11R6/lib -L/usr/local/lib -lm  -lcurl -lX11 # -pthread Flags linkado threads
##SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS). Descomentar para usar en hendrix

.PHONY:all

all: ${CLIENTE} ${SERVER}

bin/${IMDOWNLOADER}.o: src/Valla/${IMDOWNLOADER}.hpp src/Valla/${IMDOWNLOADER}.cpp
	${CC} -o ./bin/${IMDOWNLOADER}.o -c ${CPPFLAGS} src/Valla/${IMDOWNLOADER}.cpp

# SOCKETS
# Compilacion libreria de Sockets
bin/${SOCKET}.o: src/Socket/${SOCKET}.hpp src/Socket/${SOCKET}.cpp
	${CC} -o ./bin/${SOCKET}.o -c ${CPPFLAGS} src/Socket/${SOCKET}.cpp

# VALLA
# Compilacion monitor VALLA
bin/${VALLA}.o: src/Valla/${VALLA}.h src/Valla/${VALLA}.cpp
	${CC} -o ./bin/${VALLA}.o -c ${CPPFLAGS} src/Valla/${VALLA}.cpp

# VALLA
# Compilacion monitor VALLA
bin/${SUBASTA}.o: src/Subasta/${SUBASTA}.hpp src/Subasta/${SUBASTA}.cpp
	${CC} -o ./bin/${SUBASTA}.o -c ${CPPFLAGS} src/Subasta/${SUBASTA}.cpp
#-----------------------------------------------------------
# CLIENTE
# Compilacion
bin/${CLIENTE}.o: src/${CLIENTE}.cpp
	${CC} -o ./bin/${CLIENTE}.o -c  ${CPPFLAGS} src/${CLIENTE}.cpp

# Linkado
${CLIENTE}: bin/${SOCKET}.o bin/${CLIENTE}.o
	${CC} ${LDFLAGS} bin/${SOCKET}.o bin/${CLIENTE}.o -o bin/${CLIENTE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# MULTISERVIDOR
# Compilacion
bin/${SERVER}.o: src/${SERVER}.cpp
	${CC} -o ./bin/${SERVER}.o -c ${CPPFLAGS} src/${SERVER}.cpp

# Linkado
${SERVER}: bin/${SOCKET}.o bin/${VALLA}.o bin/${SUBASTA}.o bin/${IMDOWNLOADER}.o bin/${SERVER}.o
	${CC} ${LDFLAGS} bin/${SOCKET}.o bin/${VALLA}.o bin/${SUBASTA}.o bin/${IMDOWNLOADER}.o bin/${SERVER}.o -o bin/${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) bin/${SOCKET}.o
	$(RM) bin/${CLIENTE} bin/${CLIENTE}.o
	$(RM) bin/${SERVER} bin/${SERVER}.o
	$(RM) bin/${VALLA}.o
	$(RM) bin/${SUBASTA}.o
	$(RM) bin/${IMDOWNLOADER}.o
