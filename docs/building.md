---
layout: default
title: MQWeb &bull; Building MQWeb
doc_building: true
---

**This page is still a work in progress ...**

Building MQWeb
==============

POCO
----

Before you start building the MQWeb HTTP daemon, you need to build 
[POCO](http://www.pocoproject.com). It's the only dependency you need to build.
[POCO](http://www.pocoproject.com) is a collection of modern, powerful open 
source C++ class libraries and frameworks for building network- and 
internet-based applications that run on desktop, server, mobile and embedded 
systems.

MQWeb uses the 1.5.2 release. This is a development release, but it is stable to
use. [Download](http://pocoproject.org/download/index.html) and extract the
archive on your system.

It's possible to build POCO as shared or static libraries. When you plan to use
POCO for other projects, you can opt for shared libraries. I've choosen static
ones, which makes it easier to install MQWeb: there's only one executable you
need to care about.

Create a directory *static* or *shared* in the directory where POCO is 
extracted. Make this directory the current working directory and run the
 *configure* command:

for static build

    ../configure --static --omit=Data/MySQL,Data/ODBC,Data/SQLite --no-tests --no-samples
   
for shared build

    ../configure --shared --omit=Data/MySQL,Data/ODBC,Data/SQLite --no-tests --no-samples
   
The data libraries are omitted because MQWeb doesn't need them. If you only
going to use POCO for MQWeb, there is also no need to build the tests and 
samples.
