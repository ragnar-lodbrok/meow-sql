#!/bin/bash

# pre-steps: 
# 1. copy "meowsql" binary to this folder 
# 2. Download linuxdeployqt to this folder

./linuxdeployqt-continuous-x86_64.AppImage ./meowsql.desktop -verbose=2 -appimage -bundle-non-qt-libs -qmake=$HOME/Qt/5.15.0/gcc_64/bin/qmake 
