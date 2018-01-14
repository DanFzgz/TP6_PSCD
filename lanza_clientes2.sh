#!/usr/bin/env bash

EXEC="./bin/Cliente"
IP=$1
PORT=$2
URL=$3
echo "y\nn\ny\ny\ny\nn\n" |$EXEC $IP $PORT $URL

 $EXEC $IP $PORT $URL
