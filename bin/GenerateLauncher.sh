#!/bin/bash

# This tool is a part of Steam Skin Manager and
# creates a custom Launcher for Steam Skin Manager and Steam.

# It must bu launched from a Steam Skin anager root directory (with steamskinamanger executable).

selfdir="$(dirname $0))"
parentdir="$(dirname $selfdir)"
ssmdir="$parentdir"
desktop="SteamSkinManager.desktop"
output="$ssmdir/$desktop"

MACHINE_TYPE=$(uname -m)
if [ ${MACHINE_TYPE} == 'x86_64' ]; then
	ssm="steamskinmanager.64"
else
	ssm="steamskinmanager.32"
fi

echo "$ssmdir/$ssm"

if [ ! -f "$ssmdir/$ssm" ]
then
    echo "Steam Skin Manager executable is not located in this directory, exiting ..."
    exit 1
fi

function create {
	if [ -f "$output" ]
	then
		rm "$output"
	fi
	
	echo "[Desktop Entry]" >> "$output"
	echo "Version=1.0" >> "$output"
	echo "Type=Application" >> "$output"
	echo "Name=Steam Skin Manager" >> "$output"
	echo "GenericName=Theme chooser" >> "$output"
	echo "Comment=Easily change Steam's appearance" >> "$output"
	echo "Icon=$ssmdir/share/steam-skin-manager/images/logo.svg" >> "$output"
	echo "Categories=Game;Settings;Utility;" >> "$output"
	echo "" >> "$output"
	echo "Path=$ssmdir" >> "$output"
	echo "Exec=$ssmdir/$ssm" >> "$output"
	echo "" >> "$output"
	echo "Actions=SSM;Steam;SteamDecorations" >> "$output"
	echo "" >> "$output"
	echo "[Desktop Action SSM]" >> "$output"
	echo "Name=Steam Skin Manager" >> "$output"
	echo "Path=$ssmdir" >> "$output"
	echo "Exec=$ssmdir/$ssm" >> "$output"
	echo "OnlyShowIn=Unity;GNOME;" >> "$output"
	echo "" >> "$output"
	echo "[Desktop Action Steam]" >> "$output"
	echo "Name=Steam without decorations" >> "$output"
	echo "Path=$ssmdir" >> "$output"
	echo "Exec=$ssmdir/$ssm --run --no-gui" >> "$output"
	echo "OnlyShowIn=Unity;GNOME;" >> "$output"
	echo "" >> "$output"
	echo "[Desktop Action SteamDecorations]" >> "$output"
	echo "Name=Steam with decorations" >> "$output"
	echo "Path=$ssmdir" >> "$output"
	echo "Exec=$ssmdir/$ssm --run-with-decorations --no-gui" >> "$output"
	echo "OnlyShowIn=Unity;GNOME;" >> "$output"
	echo "" >> "$output"
}

echo "Creating a desktop file";
create

if [ ! -f "$output" ]
then
    echo ".desktop file could not be created, exiting ..."
    exit 1
fi

chmod +x "$output"
mv "$output" "$HOME/.local/share/applications/$desktop"

echo "Steam Skin Launcher created ... you may need to logout and login to see it in the applications menu."
exit 0
