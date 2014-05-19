#include "Category.h"

using namespace std;


void Category::SetTitle(string gained_title)
{
	title = " // " + gained_title + " // ";
	for(unsigned int i = 0; i < title.size(); i++)
	{
		underline += "‾";
	}
}

void Category::Print(ostream& os) const
{
	system("clear");
	os << title << endl << underline << endl;
}

void Category::SkipLines(ostream& os, int n) const
{
	for(int i = 0; i < n; i++)
	{
		os << endl;
	}
}

ostream& operator << (ostream& os, const Category &category)
{
    category.Print(os);
    return os;
}
