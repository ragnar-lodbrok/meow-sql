#!/bin/bash

# pre-steps: 
# 1. copy "meowsql" binary to this folder
# 2. Download linuxdeployqt to this folder

rm -rf ./input
mkdir ./input

cp ./meowsql ./input/
cp ./meowsql.desktop ./input/
cp ./meowsql.svg ./input/

./linuxdeployqt-continuous-x86_64.AppImage ./input/meowsql.desktop -verbose=2 -appimage -bundle-non-qt-libs -qmake=$HOME/Qt/5.15.0/gcc_64/bin/qmake 


# wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
# wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
# chmod +x linuxdeploy-x86_64.AppImage
# chmod +x linuxdeploy-plugin-qt-x86_64.AppImage
#export QMAKE=$HOME/Qt/5.15.0/gcc_64/bin/qmake
#./linuxdeploy-x86_64.AppImage --appdir ./input --output appimage -v1 #--plugin qt
