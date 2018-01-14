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

${IMDOWNLOADER}.o: ${IMDOWNLOADER}.hpp ${IMDOWNLOADER}.cpp
	${CC} -c ${CPPFLAGS} ${IMDOWNLOADER}.cpp

# SOCKETS
# Compilacion libreria de Sockets
${SOCKET}.o: ${SOCKET}.hpp ${SOCKET}.cpp
	${CC} -c ${CPPFLAGS} ${SOCKET}.cpp

# VALLA
# Compilacion monitor VALLA
${VALLA}.o: ${VALLA}.h ${VALLA}.cpp
	${CC} -c ${CPPFLAGS} ${VALLA}.cpp

# VALLA
# Compilacion monitor VALLA
${SUBASTA}.o: ${SUBASTA}.hpp ${SUBASTA}.cpp
	${CC} -c ${CPPFLAGS} ${SUBASTA}.cpp
#-----------------------------------------------------------
# CLIENTE
# Compilacion
${CLIENTE}.o: ${CLIENTE}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENTE}.cpp

# Linkado
${CLIENTE}: ${SOCKET}.o ${CLIENTE}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENTE}.o -o ${CLIENTE} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# MULTISERVIDOR
# Compilacion
${SERVER}.o: ${SERVER}.cpp
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp

# Linkado
${SERVER}: ${SOCKET}.o ${VALLA}.o ${SUBASTA}.o ${IMDOWNLOADER}.o ${SERVER}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${VALLA}.o ${SUBASTA}.o ${IMDOWNLOADER}.o ${SERVER}.o -o ${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CLIENTE} ${CLIENTE}.o
	$(RM) ${SERVER} ${SERVER}.o
	$(RM) ${VALLA} ${VALLA}.o
	$(RM) ${SUBASTA} ${SUBASTA}.o
	$(RM) ${IMDOWNLOADER} ${IMDOWNLOADER}.o
