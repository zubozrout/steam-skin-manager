#ifndef SKIN
#define SKIN

class Skin {
	private:
		const Settings & settings;
		
		string name;
		string path;
		string description;
		string preview_image;
		bool has_variants = false;
		
		bool FileExist(string file);
		bool CheckVariants();

	public:
		Skin(const Settings & linked_settings);
		void FillDefault(string nam);
		void Fill(string nam, string pth);
		bool HasVariants();
		string GetName();
		string GetPath(bool plain = true, bool withButtons = true);
		string GetDescription();
		string GetImg();
};

#endif /* SKIN */
