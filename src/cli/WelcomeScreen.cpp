#include "WelcomeScreen.h"

using namespace std;

WelcomeScreen::WelcomeScreen()
{
	user = getenv("USER");
}
	
void WelcomeScreen::Print(ostream& os) const
{
	Category::Print(os);
	os << "Welcome to SSM" << endl;
	os << "God day to you, " << user << endl;
	Category::SkipLines(os, 5);
}
