#include "HelpScreen.h"

#include <fstream>
#include <streambuf>

using namespace std;

Help::Help(const Settings & linked_settings): settings(linked_settings) {
	GetPath();
	ifstream ifs(filepath);
	
	help.assign((istreambuf_iterator<char>(ifs) ), (istreambuf_iterator<char>()));
}

void Help::GetPath() {
	filepath = settings.get_working_path() + "/" + settings.Key("data_path") + "doc/help.txt";
}
	
void Help::Print(ostream& os) const {
	Category::Print(os);
	os << help << endl;
}
