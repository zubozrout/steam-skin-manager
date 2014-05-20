#include <iostream>
#include <string>
#include <stdlib.h>

using namespace std;

bool gui = true;

#include "cli/Category.cpp"
#include "cli/WelcomeScreen.cpp"
#include "settings/Settings.cpp"
#include "cli/HelpScreen.cpp"
#include "steam/Steam.cpp"
#include "gui/MainWindow.cpp"

int main(int argc, char *argv[])
{	
	Settings settings;
	bool show_home = true;
	
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(argv[i] == string("--help") || argv[i] == string("-h"))
			{
				Help help(settings);
				help.SetTitle(settings.GetApplicationName() + "- HELP SCREEN");
				cout << help;
				show_home = false;
			}
			else if(argv[i] == string("--timestamp")  || argv[i] == string("-t"))
			{
				cout << "You last ran this application at " << settings.GetFileContent("last_access");
				show_home = false;
			}
			else if(argv[i] == string("--version")  || argv[i] == string("-v"))
			{
				cout << settings.GetSoftwareVersion() << endl;
				show_home = false;
			}
			else if(argv[i] == string("--run-with-decorations"))
			{
				Steam steam(settings);
				steam.Run(true);
				show_home = false;
			}
			else if(argv[i] == string("--run"))
			{
				Steam steam(settings);
				steam.Run(false);
				show_home = false;
			}
			else if(argv[i] == string("--no-gui"))
			{
				gui = false;
				if(argc <= 2)
				{
					show_home = false;
					cout << "You must call " << settings.GetApplicationName() << " with more arguments." << endl;
					cout << "No action specified ... EXITING" << endl;
				}
			}
			else if(argv[i] == string("--apply-theme"))
			{
				cout << "Setting Steam to use user-defined theme." << endl;
				settings.SetSkin(argv[i+1]);
			}
			else if(argv[i] == string("--revert-theme"))
			{
				cout << "Setting Steam to use the default theme." << endl;
				settings.RevertSkin();
			}
			else if(argv[i] == string("--generate-launcher"))
			{
				cout << "Generating .desktop file." << endl;
				settings.CreateLauncher();
			}
		}
	}
	else
	{
		if(!gui)
		{
			Help help(settings);
			help.SetTitle(settings.GetApplicationName() + "- HELP SCREEN");
			cout << help;
		}
		
		show_home = false;
	}
	
	if(show_home)
	{
		WelcomeScreen init;
		init.SetTitle(settings.GetApplicationName() + "- MAIN");
		cout << init;
	}
	
	if(gui)
	{
		MainWindow main(argc, argv, settings);
	}
	
	settings.UpdateAccessTimestamp();
	
	return EXIT_SUCCESS;
}
