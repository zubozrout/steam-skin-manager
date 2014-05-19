#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <ctime>
#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <sys/wait.h>

using namespace std;

class Category {
	protected:
		string title;
		string underline;
	public:
		void SetTitle(string gained_title)
		{
			title = " // " + gained_title + " // ";
			for(int i = 0; i < title.size(); i++)
			{
				underline += "‾";
			}
		}
		virtual void Print(ostream& os) const
        {
			system("clear");
            os << title << endl << underline << endl;
        }
        virtual void SkipLines(ostream& os, int n) const
        {
			for(int i = 0; i < n; i++)
            {
				os << endl;
			}
		}
};

ostream& operator << (ostream& os, const Category &category)
{
    category.Print(os);
    return os;
}

class WelcomeScreen : public Category {
	protected:
		string user;
		
	public:
		WelcomeScreen()
		{
			user = getenv("USER");
		}
	
		virtual void Print(ostream& os) const
        {
            Category::Print(os);
            os << "Welcome to SSM" << endl;
            os << "God day to you, " << user << endl;
            Category::SkipLines(os, 5);
        }
};

class Help : public Category {
	protected:
		string help;
		
	public:
		Help()
		{
			help = "	{\n";
			help += "	--help (-h)		will show this screen\n";
			help += "	--timestamp (-t)	will tell you when you last launched this tool\n";
			help += "	--verson (-v)		will display installed SSM version\n";
			help += "	}";
		}
	
		virtual void Print(ostream& os) const
        {
            Category::Print(os);
            os << help << endl;
        }
};

class Settings {
	private:
		string local_path;
		string config;
		string last_access;
		string application_name;
		string version;
		string system_theme;
		string system_path;
		
	public:		
		Settings()
		{
			local_path = "/home/" + string(getenv("USER")) + "/.local/share/SteamSkinManager/";
			config = local_path + "config.cfg";
			system_path = "/home/" + string(getenv("USER")) + "/Downloads/Devel/ssm4/usr/share/";
			last_access = local_path + "last_access";
			
			string command = "mkdir -p " + local_path;
			system(command.c_str());
			
			// Program parameters
			application_name = "Steam Skin Manager";
			version = "4.0.0 (Preview)";
			system_theme = CommandOutput("dconf read /org/gnome/desktop/interface/gtk-theme | sed \"s/'\\(.*\\)'/\\1/\" | tr -d \"\n\"");
		}
		
		string CommandOutput(string cmd)
		{
			string data;
			FILE * stream;
			const int max_buffer = 256;
			char buffer[max_buffer];

			stream = popen(cmd.c_str(), "r");
			if (stream)
			{
				while (!feof(stream))
				if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
				pclose(stream);
			}
			return data;
		}
		
		string GetFileContent(string file)
        {
			string lines;
			file = local_path + file;
			ifstream input(file.c_str());
            
			for(string line; getline(input, line); )
			{
				lines += line + "\n";
			}
			
			return lines;
        }
        
        string GetSoftwareVersion()
        {
			return application_name + " " + version + "\n";
        }
        
        string GetApplicationName()
        {
			return application_name;
        }
        
        void UpdateAccessTimestamp()
        {
			ofstream cfg;
			cfg.open(last_access.c_str());
			time_t t = time(0);
			struct tm * now = localtime( & t );
			cfg << now->tm_mday << "." << (now->tm_mon + 1) << "." << (now->tm_year + 1900) << "_";
			cfg << now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << endl;
			cfg.close();
		}
		
		vector<string> GetListOfAvaialbleFolders(string folder)
		{
			vector<string> list;
			
			DIR *dir;
			struct dirent *entry;

			dir = opendir(folder.c_str());
			if (dir != NULL)
			{
				while (entry = readdir(dir))
				{
					if (entry->d_type == DT_DIR && (entry->d_name != string(".") && entry->d_name != string("..")))
						list.push_back(entry->d_name);
				}
			}
			
			return list;
		}
		
		string GetSystemTheme()
		{
			return system_theme;
		}
		
		string GetPath(string type)
		{
			if(type == "theme")
				return (system_path + "steam/skins/");
			
			return NULL;
		}
};

class Steam {
	private:
		static void reap_child(int sig)
		{
			int status;
			waitpid(-1, &status, WNOHANG);
		}

	public:	
		void Run(bool native_decorations)
		{
			signal(SIGCHLD, reap_child);
			int fork_rv = fork();
			if (fork_rv == 0)
			{
				if(native_decorations)
					system("./steamdecorator.sh");
				else
					system("steam");
				_exit(1);
			}
			else if (fork_rv == -1)
			{
				cout << "Failed to fork() and run Steam" << endl;
			}
		}
};

class MainWindow {
	private:
		Settings settings;
		
		static void LaunchSteam()
		{
			cout << "Launching steam ..." << endl;
			Steam steam;
			steam.Run(true);
		}
		
		static void HelpGoHome(WebKitWebView *webview)
		{
			webkit_web_view_load_uri(webview, "http://www.iubuntu.cz/Steam/help/help.html");
		}
		
		static void HelpGoBack(WebKitWebView *webview)
		{
			webkit_web_view_go_back(webview);
		}
		
		static void HelpGoForward(WebKitWebView *webview)
		{
			webkit_web_view_go_forward(webview);
		}

	public:
		MainWindow(int argc, char* argv[], Settings &linked_settings)
		{
			settings = linked_settings;
			
			GtkBuilder *builder;
			GtkWidget *window;
			GObject *button;

			gtk_init (&argc, &argv);

			builder = gtk_builder_new ();
			gtk_builder_add_from_file(builder, "ui.glade", NULL);
			window = GTK_WIDGET(gtk_builder_get_object (builder, "window"));
			gtk_builder_connect_signals(builder, NULL);
			
			// Close window
			g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
			
			// START Object connector;
			GObject* toolbar_launchsteam;
			toolbar_launchsteam = gtk_builder_get_object(builder, "toolbar_launchsteam");
			GObject* webview_help;
			webview_help = gtk_builder_get_object(builder, "webview_help");
			GObject* url_entry;
			url_entry = gtk_builder_get_object(builder, "help_url_entry");
			GObject* help_home;
			help_home = gtk_builder_get_object(builder, "help_home_button");
			GObject* help_back;
			help_back = gtk_builder_get_object(builder, "help_back_button");
			GObject* help_forward;
			help_forward = gtk_builder_get_object(builder, "help_forward_button");
			GObject* lastrun;
			lastrun = gtk_builder_get_object(builder, "lastrunlabel");
			GObject* combo;
			combo = gtk_builder_get_object(builder, "comboboxthemes");	
			// END Object Connector
			
			// Launch steam ... toolbar button
			g_signal_connect(toolbar_launchsteam, "clicked", G_CALLBACK(LaunchSteam), NULL);
			
			// Webview ... (help)
			WebKitWebView *webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
			
			//gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(webview_help), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
			g_object_ref_sink(G_OBJECT(webview));
			gtk_container_add(GTK_CONTAINER(webview_help), GTK_WIDGET(webview));
			HelpGoHome(webview);
			//cout << "test: " << webkit_web_view_get_title(webview) << " end test" << endl;
			
			// HELP - URL ENTRY
			gtk_entry_set_text(GTK_ENTRY(url_entry), webkit_web_view_get_uri(webview));
			
			// HELP - HOME BTN
			g_signal_connect(help_home, "clicked", G_CALLBACK(HelpGoHome), webview);
			
			// HELP - Back & Forward BTN
			g_signal_connect(help_back, "clicked", G_CALLBACK(HelpGoBack), webview);
			g_signal_connect(help_forward, "clicked", G_CALLBACK(HelpGoForward), webview);
			
			// Last Run label
			gtk_label_set_text(GTK_LABEL(lastrun), ("You last ran this application at " + settings.GetFileContent("last_access")).c_str());
			
			// Combobox ... choose from installed skins
			ListAvaialbleThemes(combo, settings.GetPath("theme"));
			
			g_object_unref(webview);
			g_object_unref(G_OBJECT(builder));		

			gtk_widget_show(GTK_WIDGET(webview));
			gtk_widget_show(window);                
			gtk_main();
		}
		
		~MainWindow()
		{
			cout << "Bye Bye " << getenv("USER") << " ..." << endl;
		}
		
		void ListAvaialbleThemes(GObject* &combo, string path)
		{
			vector<string> test = settings.GetListOfAvaialbleFolders(path);
			string theme = settings.GetSystemTheme();
						
			int i = 0;
			for(vector<string>::iterator it = test.begin(); it != test.end(); ++it)
			{
				gtk_combo_box_text_insert_text(GTK_COMBO_BOX_TEXT(combo), i, (*it).c_str());
				
				if(string(*it) == theme)
					 gtk_combo_box_set_active(GTK_COMBO_BOX(combo), i);
				
				i++;
			}			
		}
};

int main(int argc, char *argv[])
{
	Settings settings;
	bool show_home = true;
	bool gui = true;
	
	if(argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			if(argv[i] == string("--help") || argv[i] == string("-h"))
			{
				Help help;
				help.SetTitle(settings.GetApplicationName() + "- HELP SCREEN");
				cout << help;
				show_home = false;
			}
			else if(argv[i] == string("--timestamp")  || argv[i] == string("-t"))
			{
				cout << "You last ran this application at " << settings.GetFileContent("last_access");
				show_home = false;
			}
			else if(argv[i] == string("--version")  || argv[i] == string("-v"))
			{
				cout << settings.GetSoftwareVersion();
				show_home = false;
			}
			else if(argv[i] == string("--run-with-decorations"))
			{
				Steam steam;
				steam.Run(true);
				show_home = false;
			}
			else if(argv[i] == string("--run"))
			{
				Steam steam;
				steam.Run(false);
				show_home = false;
			}
			else if(argv[i] == string("--no-gui"))
			{
				gui = false;
				if(argc <= 2)
				{
					show_home = false;
					cout << "You must call " << settings.GetApplicationName() << " with more arguments." << endl;
					cout << "No action specified ... EXITING" << endl;
				}
			}
		}
	}
	else
	{
		if(!gui)
		{
			Help help;
			help.SetTitle(settings.GetApplicationName() + "- HELP SCREEN");
			cout << help;
		}
		
		show_home = false;
	}
	
	if(show_home)
	{
		WelcomeScreen init;
		init.SetTitle(settings.GetApplicationName() + "- MAIN");
		cout << init;
	}
	
	if(gui)
	{
		MainWindow main(argc, argv, settings);
	}
	
	
	settings.UpdateAccessTimestamp();
	
	return EXIT_SUCCESS;
}
