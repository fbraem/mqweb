#!/bin/bash
if [ -z "$1" ]
  then
    echo "I need one argument: the root folder of the MQWeb GitHub pages"
    exit
fi
pygmentize -o $1/api/samples/queue_inq.pl.html ./queue_inq.pl
pygmentize -o $1/api/samples/queue_inq.php.html ./queue_inq.php
pygmentize -o $1/api/samples/channel_inq.pl.html ./channel_inq.pl
