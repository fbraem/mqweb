---
layout: default
title: MQWeb &bull; Building MQWeb
doc_building: true
---

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

[Download](http://pocoproject.org/download/index.html) and extract the
archive on your system.

CMake
-----

MQWeb needs [CMake](https://cmake.org) as build tool. CMake can generate
Visual Studio solutions, make files, [Codelite](http://codelite.org/)
workspaces and more.

WebSphere MQ
------------

To build MQWeb you need to install the WebSphere MQ SDK package. Make sure you
select this component when installing WebSphere MQ.

[Building MQWeb for Linux](#linux)  
[Building MQWeb for Windows](#windows)  

<a name="linux"> </a>

Linux
-----

### POCO

Create a directory *shared* in the directory where POCO is
extracted. Make this directory the current working directory and run the
 *configure* command:

    ../configure --shared --omit=Data/MySQL,Data/ODBC --no-tests --no-samples

The MySQL and ODBC data libraries are omitted because MQWeb doesn't need them. If you only
going to use POCO for MQWeb, there is also no need to build the tests and
samples.

> You can also omit the build of the XML library, when you
> define POCO\_UTIL\_NO\_XMLCONFIGURATION in *Poco/Foundation/Config.h*. You
> also need to update premake.lua in the MQWeb folder and remove the XML
> definitions.

Now you are ready to build POCO.

    make

and install ... (not required, but recommended for production)

    make install

### MQWeb

Download a MQWeb source archive from the
[releases](https://github.com/fbraem/mqweb/releases) or just download an archive
from the [source tree](https://github.com/fbraem/mqweb). Note that the
[master](https://github.com/fbraem/mqweb/tree/master) always points to the
latest release.

Premake or CMake can be used to generate makefiles, project files, ...

#### CMake

Create a build folder and make it the current directory. When running CMake the
following properties can be used:

+ CMAKE_BUILD_TYPE

  debug or release

+ POCO_INSTALLED

  set to N when POCO isn't installed. By default the build process assumes that
  POCO is installed.

+ POCO_PREFIX

  When POCO isn't installed then use this property to set the location of POCO.

Some examples:

````
cmake -DCMAKE_BUILD_TYPE=debug ..  
cmake -DCMAKE_BUILD_TYPE=debug -DPOCO_INSTALLED=N -DPOCO_PREFIX=/development/poco-1.6.1-all/shared ..
````

<a name="windows"> </a>

Windows
-------

To build MQWeb on Windows you need [Visual Studio](http://www.visualstudio.com).
MQWeb can be build with the free Express editions.

### POCO

Build the Foundation, JSON, XML, Util and Net libraries.

> You can omit the build of the XML library, when you
> define POCO\_UTIL\_NO\_XMLCONFIGURATION in *Poco/Foundation/Config.h*. Open
> premake.lua in the MQWeb folder and remove the XML definitions.

### MQWeb

Download a MQWeb source archive from the
[releases](https://github.com/fbraem/mqweb/releases) or just download an archive
from the [source tree](https://github.com/fbraem/mqweb). Note that the
[master](https://github.com/fbraem/mqweb/tree/master) always points to the
latest release, while the source tree with the latest branch points to code
that is work in progress and it is possible that it doesn't work.

#### CMake

Use CMake to generate a Visual Studio solution, codelite workspace, ...
