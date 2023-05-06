#!/bin/bash

~/Applications/Qt/5.15.2/clang_64/bin/macdeployqt ./MacOS/SV_Game.app -dmg
mv ./MacOS/SV_Game.dmg ./
rm -r ./MacOS
mkdir ./MacOS
mv SV_Game.dmg ./MacOS