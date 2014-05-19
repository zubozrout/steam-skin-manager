#ifndef SETTINGS
#define SETTINGS

class Settings {
	private:
		map<string, string> options;
		string home_path;
		string local_path;
		string config;
		string last_access;
		string system_theme;
		int current_tip = 0;
		
		string TimePartCorrector(int stamp);
				
	public:
		Settings();
		void Parse(string settings_file_path, bool console = false);
		string Key(string key) const;
		string CommandOutput(string cmd) const;
		string GetFileContent(string file) const;
        string GetSoftwareVersion() const;
        string GetApplicationName() const;
		vector<string> GetListOfAvaialbleFolders(string folder) const;
		string GetSystemTheme() const;
		string GetPath(string type) const;
		string GetSystemPath() const;
		string GetLocalPath() const;
		string GetHomePath() const;
		int StringToInt(string value);
		string GetTip(bool up);
		void UpdateAccessTimestamp();
		bool SetSkin(string path);
		bool SetInstalledSkin(string local_skin_path);
		bool RevertSkin();
		string GetCurrentTheme() const;
		string get_working_path() const;
		bool CreateLauncher();
};

#endif /* SETTINGS */
