#!/bin/sh

#tslib env
export TSLIBDIR=/mnt/nfs/opt/tslib
export TSLIB_CONSOLEDEVICE=none
export TSLIB_FBDEVICE=/dev/fb0
export TSLIB_TSDEVICE=/dev/touchscreen-1wire
export TSLIB_PLUGINDIR=$TSLIBDIR/lib/ts
export TSLIB_CONFFILE=$TSLIBDIR/etc/ts.conf
export TSLIB_CALIBFILE=$TSLIBDIR/pointercal  

#qt env
export QTDIR=/mnt/nfs/opt/qt
export QT_QWS_FONTDIR=$QTDIR/lib/fonts
export QT_PLUGIN_PATH=$QTDIR/plugins
export QWS_KEYBOARD=buttons:/dev/buttons
export QWS_MOUSE_PROTO=tslib:/dev/touchscreen_1wire
export QT_TSLIBDIR=$TSLIBDIR

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TSLIBDIR/lib:$QTDIR/lib

