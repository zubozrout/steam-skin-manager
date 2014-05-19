#include <sys/wait.h>
#include <unistd.h>

#include "Steam.h"

using namespace std;

Steam::Steam(const Settings & linked_settings): settings(linked_settings) {}

void Steam::reap_child(int sig)
{
	int status;
	waitpid(-1, &status, WNOHANG);
	Done(); // child finished (Steam closed)
}

void Steam::Run(bool native_decorations)
{
	try
	{
		signal(SIGCHLD, reap_child);
		int fork_rv = fork();
		if (fork_rv == 0)
		{
			// [TODO]: Replace system with exec
			if(native_decorations)
				system(settings.Key("steamdecorator").c_str());
			else
				system("steam");
			exit(0);
		}
		else if (fork_rv == -1)
		{
			cerr << "Failed to fork() and run Steam" << endl;
		}
	}
	
	catch (...)
	{
		cerr << "An exception occurred. Can't run Steam." << endl;
		return;
	}
}

void Steam::Done()
{
	cout << "Steam was closed." << endl;
	if(!gui) {
		cout << "Exiting Steam Skin Manager" << endl;
		exit(EXIT_SUCCESS);
	}
	// Need to find whether a class not yet defined has been instancied and send it a signal (use it's method)
}
