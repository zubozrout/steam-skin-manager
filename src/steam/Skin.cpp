#include <sys/wait.h>
#include <unistd.h>

#include "Skin.h"

using namespace std;

Skin::Skin(const Settings & linked_settings): settings(linked_settings) {}

void Skin::FillDefault(string nam) {
	name = nam;
	path = "";
	description = "<i>The is the default and official Steam theme.</i>";
	preview_image =  settings.GetSystemPath() + "/images/stock.png";
	has_variants = false;
}

void Skin::Fill(string nam, string pth) {
	if(pth == "") {
		return;
	}
	
	name = nam;
	path = pth;
	
	CheckVariants(); // Check theme's duality
	
	string desc_path = path + "/description.txt";
	if(FileExist(desc_path)) {
		description = settings.GetFileContent(desc_path);
	}
	else {
		description = "This theme has no description.";
	}
		
	string img_path = path + "/preview.png";
	if(FileExist(img_path)) {
		preview_image = img_path;
	}
	else {
		preview_image = settings.GetSystemPath() + "/images/image_missing.png";
	}
}

bool Skin::CheckVariants() {
	vector<string> test = settings.GetListOfAvaialbleFolders(path);
	int i = test.size();
	has_variants = false;
	for(vector<string>::iterator it = test.begin(); it != test.end(); ++it) {
		if((*it) == "no-buttons" || (*it) == "with-buttons") {
			has_variants = true;
		}
		i++;
	}

	return has_variants;
}

bool Skin::FileExist(string file)
{
	std::ifstream infile(file.c_str());
	return infile.good();
}

bool Skin::HasVariants() {
	return has_variants;
}

string Skin::GetName() {
	return name;
}

string Skin::GetPath(bool plain, bool withButtons) {	
	if(!plain) {
		if(has_variants) {
			if(withButtons) {
				return path + "/with-buttons";
			}
			return path + "/no-buttons";
		}
		cerr << "Theme can't have variants but not have related folders! Code ERROR!" << endl;
	}
	return path;
}

string Skin::GetDescription() {
	return description;
}

string Skin::GetImg() {
	return preview_image;
}
