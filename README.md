Kronometer README
=================

Description
-----------

Hello!
Kronometer is a simple *chronometer* application, built for the KDE Linux Desktop Environment.

Changelog
---------

v0.1

* first public and stable release
* initial basic features: start, pause, resume and reset the stopwatch widget
* basic KDE settings: toolbar/shortcuts configuration, statusbar visibility

Installation
------------

Distro-agnostic installation instructions:

1. Download the latest source files archive from github
2. Extract the archive in a folder and run the following commands:

* mkdir build && cd build
* cmake ..
* sudo make install

Now you should have kronometer installed under /usr/local/bin.

Most KDE distro should already provide the necessary software dependencies: kdelibs, cmake, make, automoc and gcc-c++.
Some distro (e.g. Sabayon) doesn't provide automoc, so it must be installed before kronometer installation.

Installation tested succesfully on:

* Chakra Linux
* Sabayon Linux (need to install automoc)

TODO list
---------

TODO