#include "HelpScreen.h"

using namespace std;

Help::Help()
{
	help = "	{\n";
	help += "	--help (-h)			will show this screen\n";
	help += "	--timestamp (-t)		will tell you when was the last time you launched this tool\n";
	help += "	--verson (-v)			will display installed SSM version\n";
	help += "	}\n";
	help += "\n";
	help += "	{\n";
	help += "	--apply-theme FOLDER_PATH	will config Steam to use theme from a defined folder\n";
	help += "	--revert-theme			will revert Steam to use the defautl Steam theme\n";
	help += "	--generate-launcher		this will create a local .desktop launcher for SSM.\n";
	help += "	}\n";
	help += "\n";
	help += "	{\n";
	help += "	--run-with-decorations		will launch Steam with system window borders\n";
	help += "	--run				will launch Steam without system window borders\n";
	help += "	--no-gui			will launch Steam Skin Manager with no gui\n";
	help += "	}\n";
}
	
void Help::Print(ostream& os) const
{
	Category::Print(os);
	os << help << endl;
}
