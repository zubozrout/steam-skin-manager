#ifndef MAIN_WINDOW
#define MAIN_WINDOW


class MainWindow {
	private:
		Glib::RefPtr<Gtk::Application> kit;
		Settings & settings;
		Skin* skin;
		future<void> *steamlaunch;
		
		bool predefined_theme;
		vector<string> bundledSkins;
		string defaultskin = "Default Skin";
		
		Gtk::Window* window;
		Gtk::Notebook* notebook;
		
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
		Gtk::Label* preview_description;
		Gtk::Image* preview_image;
		Gtk::Image* logo;
		Gtk::Spinner* spinner;
		Gtk::Viewport* frame;
		
		// Main menu
		Gtk::ImageMenuItem* menu_quit;
		Gtk::ImageMenuItem* menu_about;
		Gtk::ImageMenuItem* create_launcher;
		Gtk::ImageMenuItem* menu_run_steam;
		Gtk::ImageMenuItem* menu_run_steam_wb;
		
		// Theme chooser
		Gtk::ComboBoxText* comboboxthemes;
		Gtk::FileChooserButton* file_chooser;
		sigc::connection comboboxsignal;

		// ManualEditor and Reflection object instances
		ManualEditor* manualeditor;
		Reflection* reflection;
		
		void PageSwitched(Gtk::Widget *page, guint page_num);
		void ApplyTheme();
		void ShowTips();
		void LaunchSteam(bool decorations);
		void SteamLauncherThread(bool decorations = true);
		void SteamLauncher();
		static void SteamFinished(gpointer object);
		void ListAvaialbleThemes(string path, bool repeat = true);
		void PreviewTheme(bool native = true);
		void SpinnerStart();
		void SpinnerStop();
		void SetPreviewName();
		void SetPreviewImage();
		void NextTip();
		void PreviousTip();
		void LaunchAboutWindow();
		void CreateLauncher();
		void SetToolButtonSize(Gtk::ToolButton* button, int sizeX = -1, int sizeY = -1);
		void SetToolButtonImage(Gtk::ToolButton* button, string file);
		void Quit();
		
	public:
		MainWindow(int argc, char* argv[], Settings & linked_settings);
		~MainWindow();
};

#endif /* MAIN_WINDOW */
