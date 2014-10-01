#!/bin/bash
if [ -z "$1" ]
  then
    echo "I need one argument: the root folder of the MQWeb GitHub pages"
    exit
fi

pygmentize -o $1/api/samples/authinfo_inq.php.html ./authinfo_inq.php
pygmentize -o $1/api/samples/authinfo_inq.pl.html ./authinfo_inq.pl
pygmentize -o $1/api/samples/authrec_inq.php.html ./authrec_inq.php
pygmentize -o $1/api/samples/authrec_inq.pl.html ./authrec_inq.pl
pygmentize -o $1/api/samples/queue_inq.pl.html ./queue_inq.pl
pygmentize -o $1/api/samples/queue_inq.php.html ./queue_inq.php
pygmentize -o $1/api/samples/channel_inq.pl.html ./channel_inq.pl
pygmentize -o $1/api/samples/channel_inq.php.html ./channel_inq.php
pygmentize -o $1/api/samples/chstatus_inq.pl.html ./chstatus_inq.pl
pygmentize -o $1/api/samples/chstatus_inq.php.html ./chstatus_inq.php
pygmentize -o $1/api/samples/clusqmgr_inq.pl.html ./clusqmgr_inq.pl
pygmentize -o $1/api/samples/clusqmgr_inq.php.html ./clusqmgr_inq.php
pygmentize -o $1/api/samples/conn_inq.pl.html ./conn_inq.pl
pygmentize -o $1/api/samples/conn_inq.php.html ./conn_inq.php
