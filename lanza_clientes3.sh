#!/usr/bin/env bash

EXEC="./Cliente"
IP=$1
PORT=$2
URL=$3
echo "y\nn\ny\nn\ny\ny\n" |$EXEC $IP $PORT $URL

 $EXEC $IP $PORT $URL
