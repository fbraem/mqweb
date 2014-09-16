#!/bin/bash
if [ -z "$1" ]
  then
    echo "I need one argument: the root folder of the MQWeb GitHub pages"
    exit
fi
pygmentize -o $1/api/samples/queue.html ./queue.pl
