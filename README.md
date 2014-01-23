Kronometer README
=================

Description
-----------
Hello!

Kronometer is a simple *chronometer* application, built for the KDE Linux Desktop Environment.

Changelog
---------
v0.8.0
* Added *Edit* menu with the ability to copy current stopwatch time to clipboard
* Color settings: choose stopwatch display background/text color

v0.7.1
* Fix compilation bug

v0.7.0
* Timer central *display* re-designed
* Now each time "slot" has its own display
* The displays are *splittable* between them
* Display *time headers*, which can be disabled from settings

v0.6.0
* Initial implementation of times saving on binary files
* You can save a stopwatch *frame* (time and laps) to a file and resume it (open it) later
* Due to the new *save toolbar* (new/open/save/save as buttons), the old *main* toolbar is moved on the window bottom by default. If you don't like it, you can always move it by right-clicking on it
* Added **Kronometer** menu, separating the *save* actions (menu File) from the *stopwatch* actions
* Save settings. You can disable the dialog on application exit if there are unsaved times

v0.5.0
* Now you can choose also the time format for lap times
* Font settings for timer display font
* Fixed general settings appereance, with a scrollable layout

v0.4.0
* Initial implementantation of Kronometer settings dialog (Settings -> Configure Kronometer)
* Stopwatch time format settings: show or not show hours/minutes/seconds/tenths/hundredths/milliseconds
* Now you can resize time label and lap times table without resize the application window

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

* Gentoo
* Arch Linux
* Chakra Linux
* Sabayon Linux (need to install **automoc**)

TODO list
---------

* ~~UI basic improvements: widget separator, labels, etc.~~
* ~~*Laps* recording feature~~
* ~~Granularity: ability to choose chronometer granularity~~
* ~~Save support: ability to save/restore times~~
* ~~Chronometer font customization: font famility, size, etc.~~
* ~~Various UI enhancements~~
* ~~Copy current time to clipboard~~
* ~~Color settings~~
* Replace binary files with text files for persistence.
* Laps export feature: CSV format, etc.
* Text localization
* System tray support? (pause/resume from tray icon). Hard because multiple windows may be running.

Screenshot
----------
<!-- old relative screenshot links
[![kronometer inactive](img/screen-v0.3.x/kronometer-inactive.png?raw=true)](img/screen-v0.3.x/kronometer-inactive.png?raw=true)
[![kronometer running](img/screen-v0.3.x/kronometer-running.png?raw=true)](img/screen-v0.3.x/kronometer-running.png?raw=true)
[![kronometer paused](img/screen-v0.3.x/kronometer-paused.png?raw=true)](img/screen-v0.3.x/kronometer-paused.png?raw=true)
[![kronometer paused](img/screen-v0.3.x/kronometer-laps.png?raw=true)](img/screen-v0.3.x/kronometer-laps.png?raw=true)
-->
[![kronometer running](http://abload.de/img/kronometer-running-lan4ij0.png)](http://abload.de/img/kronometer-running-lan4ij0.png)
[![kronometer save](http://abload.de/img/kronometer-saveggs6g.png)](http://abload.de/img/kronometer-saveggs6g.png)
[![kronometer settings](http://abload.de/img/kronometer-settingsqueub.png)](http://abload.de/img/kronometer-settingsqueub.png)


