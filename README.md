Kronometer README
=================

Description
-----------
Hello!

Kronometer is a simple *chronometer* application, built for the KDE Linux Desktop Environment.

Changelog
---------
v0.3.0
* Lap recording feature
* Lap times sorting

v0.2.0
* *Panel-like* look for time label
* Time format in statusbar
* Tooltip messages for labels in statusbar

v0.1.2
* Switched install prefix from (cmake-default) **/usr/local** to **/usr**

v0.1.1
* Added **.desktop** file for KMenu integration

v0.1.0
* First public and stable release
* Initial basic features: start, pause, resume and reset the stopwatch widget
* Basic KDE settings: toolbar/shortcuts configuration, statusbar visibility

Installation
------------

Distro-agnostic installation instructions:

1. Download the latest source files archive from github
2. Extract the archive in your preferred directory and run the following commands:

```
$ mkdir build && cd build
$ cmake -DCMAKE_INSTALL_PREFIX=`kde4-config --prefix` ..
$ sudo make install
```

Now you should have **kronometer** installed under **/usr/bin** and it should be available under the *Utility* entry in your KMenu.
If not, try to reboot your pc and now **kronomoter** should be in your KMenu.

Most KDE distro should already provide the necessary software dependencies: **kdelibs**, **cmake**, **make**, **automoc** and **gcc-c++**.
Some distro (e.g. Sabayon) might not provide **automoc** by default, so it must be installed before kronometer installation.

Installation tested succesfully on:

* Chakra Linux
* Sabayon Linux (need to install **automoc**)

TODO list
---------

* ~~UI basic improvements: widget separator, labels, etc.~~
* ~~*Laps* recording feature~~
* Granularity: ability to choose chronometer granularity
* Save support: ability to save/restore times
* Chronometer font customization: font famility, size, etc.
* Text localization

Screenshot
----------

<!-- [![kronometer inactive](img/screen-v0.3.x/kronometer-inactive.png?raw=true)](img/screen-v0.3.x/kronometer-inactive.png?raw=true) -->
<!--[![kronometer running](img/screen-v0.3.x/kronometer-running.png?raw=true)](img/screen-v0.3.x/kronometer-running.png?raw=true)
[![kronometer paused](img/screen-v0.3.x/kronometer-paused.png?raw=true)](img/screen-v0.3.x/kronometer-paused.png?raw=true)
[![kronometer paused](img/screen-v0.3.x/kronometer-laps.png?raw=true)](img/screen-v0.3.x/kronometer-laps.png?raw=true)-->
[![kronometer running](http://kde-apps.org/CONTENT/content-pre1/162829-1.png)](http://kde-apps.org/CONTENT/content-pre1/162829-1.png)
[![kronometer paused](http://kde-apps.org/CONTENT/content-pre2/162829-2.png)](http://kde-apps.org/CONTENT/content-pre2/162829-2.png)
[![kronometer paused](http://kde-apps.org/CONTENT/content-pre3/162829-3.png)](http://kde-apps.org/CONTENT/content-pre3/162829-3.png)


