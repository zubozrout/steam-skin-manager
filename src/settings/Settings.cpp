#include <fstream>
#include <iostream>
#include <stdexcept>
#include <map>
#include <vector>
#include <ctime>
#include <sys/wait.h>
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Settings.h"

using namespace std;

string Settings::TimePartCorrector(int stamp)
{
	if(stamp < 10)
		return ("0" + to_string(stamp));
	else
		return to_string(stamp);
}

Settings::Settings()
{
	Parse("settings.conf", !gui);
	
	home_path = "/home/" + string(getenv("USER"));
	local_path = home_path + "/" + Key("user_path");
	config = local_path + "config.cfg";
	last_access = local_path + "last_access";
	
	string command = "mkdir -p " + local_path;
	system(command.c_str());
	
	// Program parameters
	system_theme = CommandOutput(Key("system_theme"));
}

// Source: http://stackoverflow.com/questions/6892754/creating-a-simple-configuration-file-and-parser-in-c
void Settings::Parse(string file_path, bool console)
{
	if(console) {
		cout << "Parsing " << file_path << endl;
	}
	
	ifstream file(file_path.c_str());
	string id, eq, val, line;
	while(getline(file, line))
	{
		istringstream iss(line);
		if(iss >> id >> eq >> val)
		{
			if (id[0] == '#')
			{
				continue;  // skip comments
			}
			if (eq != "=")
			{
				cerr << "[ERROR] " << "Mischmasch in the file." << endl;
				continue;
			}
			
			char on_line;
			while(iss.get(on_line))
				val += on_line;
			
			if(console) {
				cout << "[OK]   	" << id << " = " << val << endl;
			}
			options[id] = val;
		}
	}	
	file.close();
}

string Settings::Key(string key) const
{
	map<string, string>::const_iterator it = options.find(key);
	if (it != options.end())
	{
		return it->second;
	}
	else
	{
		throw runtime_error("Key not found");
	}
}

int Settings::StringToInt(string value)
{
	istringstream num_stream(value);
	int i;
	num_stream >> i;
	return i;
}

string Settings::GetTip(bool up)
{
	int entries = StringToInt(Key("num_of_tips"));
	
	try
	{
		options["tip" + to_string(entries + 1)] = CommandOutput("echo -n \"$(wget iubuntu.cz/Steam/variable_content/tip.php -q -O -)\"");
		if(Key(string("tip") + to_string(entries + 1)) != "")
			entries++;
	}
	catch(...)
	{
		cerr << "Can't load remote tip from network." << endl;
	}
			
	if(up)
		current_tip++;
	else
		current_tip--;
	
	if(current_tip > entries)
		current_tip = 1;
	if(current_tip < 1)
		current_tip = entries;
	
	return Key(string("tip") + to_string(current_tip));
}

string Settings::CommandOutput(string cmd) const
{
	string data;
	FILE * stream;
	const int max_buffer = 256;
	char buffer[max_buffer];

	stream = popen(cmd.c_str(), "r");
	if (stream)
	{
		while (!feof(stream))
		if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		int ret = pclose(stream)/256;
		if(ret != 0)
			cerr << "Command could not be executed, error code: " << ret << endl; // error
	}
	return data;
}

string Settings::GetFileContent(string file) const
{
	string lines = "";
	try
	{
		ifstream input(file.c_str());
		
		if (input.is_open())
		{
			for(string line; getline(input, line); )
			{
				if(line != "")
					lines += line + "\n";
			}
			input.close();
		}
		else
			cerr << "Can't open " << file << " file." << endl;
	}
	catch(...)
	{
		cerr << "An exception was thrown due to a problem reading a file" << endl;
	}
	return lines;
}

string Settings::GetSoftwareVersion() const
{
	return Key("version");
}

string Settings::GetApplicationName() const
{
	return Key("app_name");
}

vector<string> Settings::GetListOfAvaialbleFolders(string folder) const
{
	vector<string> list;
	
	DIR *dir;
	struct dirent *entry;

	dir = opendir(folder.c_str());
	if (dir != NULL)
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR && (entry->d_name != string(".") && entry->d_name != string("..")))
				list.push_back(entry->d_name);
		}
	}
	
	return list;
}

string Settings::GetSystemTheme() const
{
	return system_theme;
}

string Settings::GetPath(string type) const
{
	if(type == "theme")
		return Key("themes_path");
	
	return NULL;
}

string Settings::GetLocalPath() const
{
	return local_path;
}

string Settings::GetHomePath() const
{
	return home_path;
}

string Settings::GetSystemPath() const
{
	return Key("data_path");
}

void Settings::UpdateAccessTimestamp()
{
	try
	{
		ofstream cfg;
		cfg.open(last_access.c_str());
		time_t t = time(0);
		struct tm * now = localtime( & t );
		cfg << now->tm_mday << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << "_";
		cfg << TimePartCorrector(now->tm_hour) << ":" << TimePartCorrector(now->tm_min) << ":" << TimePartCorrector(now->tm_sec) << endl;
		cfg.close();
	}
	catch (...)
	{
		cerr << "Can't write to a file.";
	}
}

bool Settings::SetSkin(string path)
{
	string local_command = Key("steamskinapplier") + " -i " + path;
	cout << "Executing: " << local_command << endl;
	int status_code = system(local_command.c_str());
    if(status_code != -1 && status_code != 127)
		return true;
	return false;
}

bool Settings::SetInstalledSkin(string local_skin_path)
{
	string local_command = Key("steamskinapplier") + " -i " + get_working_path() + "/" + Key("themes_path") + local_skin_path;
	cout << "Executing: " << local_command << endl;
	int status_code = system(local_command.c_str());
	if(status_code != -1 && status_code != 127  && status_code != 1)
		return true;
	return false;
}

bool Settings::RevertSkin()
{
	string local_command = Key("steamskinapplier") + " -r";
	cout << "Executing: " << local_command << endl;
	int status_code = system(local_command.c_str());
	if(status_code != -1 && status_code != 127)
		return true;
	return false;
}

string Settings::GetCurrentTheme() const
{	
	return CommandOutput(Key("steamskinapplier") + " -g");
}

string Settings::get_working_path() const
{
   char temp[100];
   return (getcwd(temp, 100) ? string(temp) : string(""));
}

bool Settings::CreateLauncher() {
	if(system((get_working_path() + "/bin/GenerateLauncher.sh").c_str())) {
		return true;
	}
	return false;
}
