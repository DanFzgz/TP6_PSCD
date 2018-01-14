#!/usr/bin/env bash

if [ "$#" -ne 1 ]; then
  echo "./$0 <num_clients>"
  exit 1
fi

EXEC="./Servidor"
PORT=$1;

 $EXEC $PORT
