#!/usr/bin/env bash

if [ "$#" -ne 3 ]; then
  echo "./$0 <num_clients>"
  exit 1
fi

EXEC="./bin/Cliente"
SERVER=$1
PORT=$2;
URL=$3;

 $EXEC $SERVER $PORT $URL
