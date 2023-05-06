#!/bin/bash

# Not working (can not find gss path)
#D:/Programmes/Qt/5.15.2/mingw81_64/bin/windeployqt ./Windows/release/SV_Game.exe

mv ./Windows/release ./
rm -r ./Windows
mv release Windows
rm -f ./Windows/*.h ./Windows/*.cpp ./Windows/*.o