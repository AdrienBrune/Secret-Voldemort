#!/bin/bash

~/Applications/Qt/5.15.2/clang_64/bin/macdeployqt ./MacOS/SV_Server.app -dmg
mv ./MacOS/SV_Server.dmg ./
rm -r ./MacOS
mkdir ./MacOS
mv SV_Server.dmg ./MacOS