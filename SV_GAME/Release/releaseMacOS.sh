#!/bin/bash

if [ test -f "/MacOS/SV_Game.dmg" ]
then
	echo Impossible to release. Remove dmg file before releasing ...
	sleep 4
else
	if [ ~/Applications/Qt/5.15.2/clang_64/bin/macdeployqt ./MacOS/SV_Game.app -dmg ]
	then
		mv ./MacOS/SV_Game.dmg ./
		rm -r ./MacOS
		mkdir ./MacOS
		mv SV_Game.dmg ./MacOS
		echo Release success ...
		sleep 4
	else
		echo macdeployqt command failed ...
		sleep 4
	fi
fi