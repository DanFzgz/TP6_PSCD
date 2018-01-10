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
CLIENT=Cliente
SERVER=Servidor
VALLA=Valla
SUBASTA=Subasta
imageDownloader=ImageDownloader

# #################### #
# FLAGS DE COMPILACION #
# #################### #
CPPFLAGS=-I. -O2 -std=c++11 -lsockets # Flags compilacion
LDFLAGS=-pthread # Flags linkado threads
##SOCKETSFLAGS=-lsocket -lnsl # Flags linkado sockets (Solaris SunOS). Descomentar para usar en hendrix

.PHONY:all

all: ${CLIENT} ${SERVER}

${imageDownloader}.o: ${imageDownloader}.hpp ${imageDownloader}.cpp
	${CC} -c ${CPPFLAGS} ${imageDownloader}.cpp

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
${CLIENT}.o: ${CLIENT}.cpp
	${CC} -c ${CPPFLAGS} ${CLIENT}.cpp

# Linkado
${CLIENT}: ${SOCKET}.o ${CLIENT}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${CLIENT}.o -o ${CLIENT} ${SOCKETSFLAGS}
#-----------------------------------------------------------
# MULTISERVIDOR
# Compilacion
${SERVER}.o: ${SERVER}.cpp
	${CC} -c ${CPPFLAGS} ${SERVER}.cpp

# Linkado
${SERVER}: ${SOCKET}.o ${VALLA}.o ${SUBASTA}.o ${imageDownloader}.o ${SERVER}.o
	${CC} ${LDFLAGS} ${SOCKET}.o ${VALLA}.o ${SUBASTA}.o ${imageDownloader}.o ${SERVER}.o -o ${SERVER} ${SOCKETSFLAGS}
#-----------------------------------------------------------

# LIMPIEZA
clean:
	$(RM) ${SOCKET}.o
	$(RM) ${CLIENT} ${CLIENT}.o
	$(RM) ${SERVER} ${SERVER}.o
	$(RM) ${VALLA} ${VALLA}.o
	$(RM) ${SUBASTA} ${SUBASTA}.o
