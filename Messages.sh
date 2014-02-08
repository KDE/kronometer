#! /bin/sh
$EXTRACTRC `find . -name \*.rc -o -name \*.kcfg -o -name \*.ui` >> rc.cpp || exit 11
$XGETTEXT `find . -name \*.h -o -name \*.cpp` -o $podir/kronometer.pot
