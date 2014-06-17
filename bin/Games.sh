#!/bin/sh

DIRECTORY=~/.steam/root/SteamApps
find $DIRECTORY -maxdepth 1 -type f -exec cat {} \; | grep -i "name" | sed 's/name//' | tr -d "\t\""
