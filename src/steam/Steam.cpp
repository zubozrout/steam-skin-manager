#include <sys/wait.h>
#include <unistd.h>

#include "Steam.h"

using namespace std;

Steam::Steam(const Settings & linked_settings): settings(linked_settings) {}

void Steam::Run(bool native_decorations) {
	try {
		int pid = fork();
		if(pid == -1) {
			cerr << "Failed to fork() and run Steam" << endl;
		}
		else if(pid > 0) {
			int status;
			waitpid(pid, &status, 0);
			return;
		}
		else {
			// pid == 0
			if(native_decorations)
				system((settings.get_working_path() + settings.Key("steamdecorator")).c_str());
			else
				system("steam");
			exit(0);
		}
	}
	
	catch (...) {
		cerr << "An exception occurred. Can't run Steam." << endl;
		return;
	}
}

Steam::~Steam() {
	if(!gui) {
		cout << "Exiting Steam Skin Manager" << endl;
		exit(EXIT_SUCCESS);
	}
}
