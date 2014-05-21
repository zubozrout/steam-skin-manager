#ifndef MAIN_WINDOW
#define MAIN_WINDOW


class MainWindow {
	private:
		Settings & settings;
		bool predefined_theme;
		vector<string> bundledSkins;
		
		Gtk::Window* window;
		
		Glib::RefPtr<Gtk::Builder> builder;
		Gtk::Toolbar* toolbar;
		Gtk::ToolButton* launch_steam_from_toolbar;
		Gtk::Button* prev_tip;
		Gtk::Button* next_tip;
		Gtk::Label* header;
		Gtk::Label* lastrun;
		Gtk::Label* notify;
		Gtk::Button* apply;
		Gtk::Switch* use_decorations;
		Gtk::FileChooserButton* file_chooser;
		Gtk::Label* preview_description;
		Gtk::Image* preview_image;
		Gtk::Image* logo;
		Gtk::Spinner* spinner;
		
		// Main menu
		Gtk::ImageMenuItem* menu_about;
		Gtk::ImageMenuItem* create_launcher;
		
		// Theme chooser
		Gtk::ComboBoxText* comboboxthemes;
		
		// ManualEditor object instance
		ManualEditor* manualeditor;
		
		void ApplyTheme();
		void ShowTips();
		void LaunchSteam();
		void ListAvaialbleThemes(string path);
		void PreviewTheme(bool preinstalled);
		void SpinnerStart();
		void SpinnerStop();
		void SetPreviewImage(string path, string descritption);
		void NextTip();
		void PreviousTip();
		void LaunchAboutWindow();
		void CreateLauncher();
		
	public:
		MainWindow(int argc, char* argv[], Settings & linked_settings);
		~MainWindow();
};

#endif /* MAIN_WINDOW */
