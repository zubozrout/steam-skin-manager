#ifndef MAIN_WINDOW
#define MAIN_WINDOW


class MainWindow {
	private:
		Glib::RefPtr<Gtk::Application> kit;
		Settings & settings;
		Skin* skin;
		future<void> *steamlaunch;
		Glib::Mutex mutex;
		
		bool predefined_theme;
		vector<string> bundledSkins;
		string defaultskin = "Default Skin";
		
		Gtk::Window* window;
		
		Glib::RefPtr<Gtk::Builder> builder;
		Gtk::Toolbar* toolbar;
		Gtk::ToolButton* launch_steam_from_toolbar;
		Gtk::ToolButton* prev_tip;
		Gtk::ToolButton* next_tip;
		Gtk::Label* header;
		Gtk::Label* lastrun;
		Gtk::Label* notify;
		Gtk::Label* skin_name_label;
		Gtk::EventBox* preview_name_box;
		Gtk::Button* apply;
		Gtk::Switch* use_decorations;
		Gtk::FileChooserButton* file_chooser;
		Gtk::Label* preview_description;
		Gtk::Image* preview_image;
		Gtk::Image* logo;
		Gtk::Spinner* spinner;
		
		// Main menu
		Gtk::ImageMenuItem* menu_quit;
		Gtk::ImageMenuItem* menu_about;
		Gtk::ImageMenuItem* create_launcher;
		Gtk::ImageMenuItem* menu_run_steam;
		Gtk::ImageMenuItem* menu_run_steam_wb;
		
		// Theme chooser
		Gtk::ComboBoxText* comboboxthemes;
		
		// ManualEditor object instance
		ManualEditor* manualeditor;
		
		void ApplyTheme();
		void ShowTips();
		void LaunchSteam(bool decorations);
		void SteamLauncherThread(bool decorations = true);
		void SteamLauncher();
		static void SteamFinished(gpointer object);
		void ListAvaialbleThemes(string path);
		void PreviewTheme(bool native = true);
		void SpinnerStart();
		void SpinnerStop();
		void SetPreviewName();
		void SetPreviewImage();
		void NextTip();
		void PreviousTip();
		void LaunchAboutWindow();
		void CreateLauncher();
		void Quit();
		
	public:
		MainWindow(int argc, char* argv[], Settings & linked_settings);
		~MainWindow();
};

#endif /* MAIN_WINDOW */
