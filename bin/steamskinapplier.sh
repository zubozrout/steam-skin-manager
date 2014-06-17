#!/bin/bash


#	Ubuntu Steam Integration Tool
#	This tool is an automated installer and uninstaller.
#	
#	Created by Martin Kozub <zubozrout@gmail.com>
#	The tool and the skins are free to use, share and edit - under the terms of CC-BY license (Creative Commons).
#
#	Develper mode ...

# PREDEFINED PATHS:
SKINPATH=$2
LOCALFILES=/usr/share/steam/skins
STEAMSKIN=~/.steam/root/skins
STEAMSETTINGS=~/.steam/registry.vdf

TOOL_NAME="Ubuntu Steam Integration Tool"


################## HELP ##################

function print_help {
	clear
	echo ""
	echo "		Welcome to $TOOL_NAME"
	echo ""
	echo ""
	echo "	This application can install and apply skins to your Steam client."
	echo ""
	echo "	Usage:"
	echo "	-i [path]	installs and applies the skin on the defined path."
	echo "	-r, -u		removes currently active skin."
	echo "	-c		checks for inconsistency with the desktop theme. (*)"
	echo "	-g		retrieves the name of the currently used theme"
	echo ""
	echo "	(*) If the skin uses the same name as the system theme,"
	echo "	this test passes."
	echo ""

	exit 0
}

################## INSTALLER ##################

function extract_skin_name {
	if ( echo "$SKINPATH" | grep "\-buttons" ) ; then
		SKINNAME=`echo "$SKINPATH" | sed "s/^.*\/\(.*\/.*$\)/\1/"`
	else
		SKINNAME=`echo "$SKINPATH" | sed "s/^.*\/\(.*$\)/\1/"`
	fi

	if [ "$SKINNAME" == "$SKINPATH" ] ; then
		SKINPATH="$LOCALFILES/$SKINNAME"
	fi

	SKINNAME=`echo "$SKINNAME" | sed 's/\(.*\)\/with-buttons/\1/'`
	SKINNAME=`echo "$SKINNAME" | sed 's/\(.*\)\/no-buttons/\1/'`

	return 0
}

function is_installed {
	if ! [ -e "$STEAMSKIN/$SKINNAME" ] ; then
		return 1
	elif ! ( `grep "\"SkinV4\".*\"$SKINNAME\"" $STEAMSETINGS` ) ; then
		return 2
	fi
	
	return 0
}

function install_skin {
	echo "Installing $SKINNAME"

	# Test removed. Now installs under all circumstancies.	

	echo "Linking defined skin with Steam installation"

	rm -f "$STEAMSKIN/$SKINNAME" 2> /dev/null

	echo "New route: $STEAMSKIN/$SKINNAME"
	mkdir -p "$STEAMSKIN/"

	if ! ( ln -s "$SKINPATH" "$STEAMSKIN/$SKINNAME" ) ; then
		echo "Can not create the link!"
		zenity --warning --title "An error occured" --text "The installator couldn't connect your Steam installation with the theme. Please copy or link the directory manually:\n\n\"$SKINPATH\" >> \"$STEAMSKIN\"."
		echo "Continuing to config the settings."
		exit 1
	fi

	echo "Setting config file"

	if ( grep '"SkinV4"' "$STEAMSETTINGS" ) ; then
		sed "/\"SkinV4\"/d" "$STEAMSETTINGS" > ~/tmpfile
		mv ~/tmpfile "$STEAMSETTINGS"
	fi

	# Real file configuration (inserting skin config line into a settings file)
	NL=`grep -n '"HKCU"' "$STEAMSETTINGS" | cut -d: -f1`
	NT="$(tail -$((`cat "$STEAMSETTINGS" | wc -l` - $NL)) "$STEAMSETTINGS" | grep -n '"Steam"' | cut -d: -f1)"
	EQ=$((`cat $STEAMSETTINGS | wc -l` - $NL - $NT - 1))
	tail -$EQ "$STEAMSETTINGS" > ~/tmpfile | (head -$(($NL + NT + 1)) "$STEAMSETTINGS"
	echo "					\"SkinV4\"		\"$SKINNAME\""
	cat ~/tmpfile) > ~/tmpfile2
	mv ~/tmpfile2 "$STEAMSETTINGS"
	rm ~/tmpfile

	echo "Skin succesfully applied."

	return 0
}


# MAIN
function run_installer {
	echo "the installer was called with this parameter: $SKINPATH"

	if ! [ -e "$SKINPATH" ] ; then
		echo "The skin does not extist."
	fi

	extract_skin_name
	install_skin

	exit 0
}

################## UNINSTALLER ##################

function remove {
	if ( grep '"SkinV4"' "$STEAMSETTINGS" ) ; then
		SKINNAME=`sed -n "s/^.*\"SkinV4\".*\"\(.*\)\"$/\1/p" "$STEAMSETTINGS"`
	
		echo "Removing the $SKINNAME from Steam installation."
	
		if ! ( rm -f "$STEAMSKIN/$SKINNAME" ) ; then
			echo "An error occured while trying to remove the skin. Do you have sufficient rights for that?"
			exit 1
		fi

		echo "Removing the skin settings from the config file"

		sed "/\"SkinV4\"/d" "$STEAMSETTINGS" > ~/tmpfile
		mv ~/tmpfile "$STEAMSETTINGS"
	fi
}

################## GETTER ##################

function get {
	if ( grep '"SkinV4"' "$STEAMSETTINGS" > /dev/null ) ; then
		SKINNAME=`sed -n "s/^.*\"SkinV4\".*\"\(.*\)\"$/\1/p" "$STEAMSETTINGS"`
	
		echo -n "$SKINNAME"
	fi
}

################## VALIDATOR ##################

function check {
	if ( grep '"SkinV4"' "$STEAMSETTINGS" > /dev/null ) ; then
		SKINNAME=`sed -n "s/^.*\"SkinV4\".*\"\(.*\)\"$/\1/p" "$STEAMSETTINGS"`
	fi

	if ( dconf read /org/gnome/desktop/interface/gtk-theme | grep "$SKINNAME" > /dev/null ) ; then
		echo "You are using the right theme.			[ OK ]"
		exit 0
	else
		echo "You are using the different theme than $SKINNAME.	[ ER ]"
		exit 1
	fi
}

################## END ##################


################## MAIN ##################

if [ -z $1 ] ; then
	print_help
fi

# Switch to a home directory ... better chance to be able to have write permissions when needed.
cd ~

	###########################################################
if [ $1 == "-i" ] ; then
	if [ -z "$SKINPATH" ] ; then print_help ; fi
	echo "Installing skin ..."
	run_installer
	###########################################################
elif [ $1 == "-u" -o $1 == "-r" ] ; then
	echo "Removing skin currently in use ..."
	remove
	###########################################################
elif [ $1 == "-c" ] ; then
	echo "Checking the consistency ..."
	check
	###########################################################
elif [ $1 == "-g" ] ; then
	get
	###########################################################
else
	print_help
fi
	###########################################################

exit 0

