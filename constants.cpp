#include <iostream>
#include <pwd.h>
#include <unistd.h>

using namespace std;

// This file contains global SSM constants.

// Predefined srtings:
const string bundled_skins_path = "/usr/share/steam/skins/";
const string skins_with_buttons = "with-buttons";
const string skins_without_buttons = "no-buttons";
const string steam_path = ".steam";
const string registry_name = "registry.vdf";

string install_skin_command = "steam-skin -i ";
string revert_skin_command = "steam-skin -r";

const char* execution_error = "Could not execute a command";
const char* steam_execution_error = "Could not launch Steam";
const char* custom_not_set = "You must set a directory to link with!";

const string user_path = string(getpwuid(getuid())->pw_dir);
const string user_name = string(getpwuid(getuid())->pw_name);
const string complete_path = user_path + '/' + steam_path;

const string app_version = "3.9.1";
