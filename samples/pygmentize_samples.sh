#!/bin/bash
if [ -z "$1" ]
  then
    echo "I need one argument: the root folder of the MQWeb GitHub pages"
    exit
fi

pygmentize -o $1/api/samples/php/authinfo_inq.html ./php/authinfo_inq.php
pygmentize -o $1/api/samples/perl/authinfo_inq.html ./perl/authinfo_inq.pl
pygmentize -o $1/api/samples/php/authrec_inq.html ./php/authrec_inq.php
pygmentize -o $1/api/samples/perl/authrec_inq.html ./perl/authrec_inq.pl
pygmentize -o $1/api/samples/php/queue_inq.html ./php/queue_inq.php
pygmentize -o $1/api/samples/perl/queue_inq.html ./perl/queue_inq.pl
pygmentize -o $1/api/samples/ruby/queue_inq.html ./ruby/queue_inq.rb
pygmentize -o $1/api/samples/php/channel_inq.html ./php/channel_inq.php
pygmentize -o $1/api/samples/perl/channel_inq.html ./perl/channel_inq.pl
pygmentize -o $1/api/samples/php/chstatus_inq.html ./php/chstatus_inq.php
pygmentize -o $1/api/samples/perl/chstatus_inq.html ./perl/chstatus_inq.pl
pygmentize -o $1/api/samples/php/clusqmgr_inq.html ./php/clusqmgr_inq.php
pygmentize -o $1/api/samples/perl/clusqmgr_inq.html ./perl/clusqmgr_inq.pl
pygmentize -o $1/api/samples/php/conn_inq.html ./php/conn_inq.php
pygmentize -o $1/api/samples/perl/conn_inq.html ./perl/conn_inq.pl
pygmentize -o $1/api/samples/php/listener_inq.html ./php/listener_inq.php
pygmentize -o $1/api/samples/perl/listener_inq.html ./perl/listener_inq.pl
pygmentize -o $1/api/samples/php/lsstatus_inq.html ./php/lsstatus_inq.php
pygmentize -o $1/api/samples/perl/lsstatus_inq.html ./perl/lsstatus_inq.pl
pygmentize -o $1/api/samples/php/namelist_inq.html ./php/namelist_inq.php
pygmentize -o $1/api/samples/perl/namelist_inq.html ./perl/namelist_inq.pl
pygmentize -o $1/api/samples/php/process_inq.html ./php/process_inq.php
pygmentize -o $1/api/samples/perl/process_inq.html ./perl/process_inq.pl
pygmentize -o $1/api/samples/php/service_inq.html ./php/service_inq.php
pygmentize -o $1/api/samples/perl/service_inq.html ./perl/service_inq.pl
pygmentize -o $1/api/samples/php/qmgr_inq.html ./php/qmgr_inq.php
pygmentize -o $1/api/samples/perl/qmgr_inq.html ./perl/qmgr_inq.pl
pygmentize -o $1/api/samples/php/qmstatus_inq.html ./php/qmstatus_inq.php
pygmentize -o $1/api/samples/perl/qmstatus_inq.html ./perl/qmstatus_inq.pl

