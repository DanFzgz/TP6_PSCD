#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "./$0 <num_clients>"
  exit 1
fi

IP=$1;
PORT=$2;
URL=$3
SCRIPT1="../lanza_clientes1.sh"
SCRIPT2="../lanza_clientes2.sh"
SCRIPT3="../lanza_clientes3.sh"

	xterm -e $SCRIPT1 $IP $PORT $URL &
	xterm -e $SCRIPT2 $IP $PORT $URL & 
	xterm -e $SCRIPT3 $IP $PORT $URL &