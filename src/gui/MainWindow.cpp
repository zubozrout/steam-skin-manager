#include <gtkmm.h>
#include <random>

#include "WebView.cpp"
#include "ManualEditor.cpp"
#include "AboutWindow.cpp"

#include "MainWindow.h"


using namespace std;

MainWindow::MainWindow(int argc, char* argv[], Settings & linked_settings): settings(linked_settings)
{
	Gtk::Main kit(argc, argv);
	
	try
	{
		builder = Gtk::Builder::create_from_file(settings.Key("ui_path"));
	}  
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
	}  
	
	#include "MainWindowBuilder.connect"
	
	// Main menu
	menu_about->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::LaunchAboutWindow));
	create_launcher->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::CreateLauncher));
	
	// Toolbar
	Glib::RefPtr<Gtk::StyleContext> sc = toolbar->get_style_context();
	sc->add_class("primary-toolbar");
	// Toolbar buttons
	launch_steam_from_toolbar->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LaunchSteam));
	lastrun->set_text(("You last ran this application at " + settings.GetFileContent(settings.GetLocalPath() + "last_access") + "You are currently using " + settings.GetCurrentTheme() + " theme.").c_str());
	
	// Combobox settings
	ListAvaialbleThemes(settings.GetPath("theme"));
	// Apply theme button
	apply->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::ApplyTheme));
	
	// File chooser
	comboboxthemes->signal_changed().connect(sigc::group(sigc::mem_fun(*this, &MainWindow::PreviewTheme), true));
	file_chooser->signal_file_set().connect(sigc::group(sigc::mem_fun(*this, &MainWindow::PreviewTheme), false));
	
	header->set_markup("Welcome to " + settings.GetApplicationName() + " " + settings.GetSoftwareVersion());
	header->override_font(Pango::FontDescription("18px"));
	
	// Load WebView
	WebView web(builder, settings);
	// Manual Steam Settings Edit
	//ManualEditor(builder, settings);
	manualeditor = new ManualEditor(builder, settings);
	
	PreviewTheme(true);
    ShowTips();
    	
	if(window)
	{
		window->set_title(settings.GetApplicationName().c_str());
		Gtk::Main::run(*window);
	}
}

void MainWindow::LaunchAboutWindow()
{
	AboutWindow about(builder, settings);
}

void MainWindow::ApplyTheme()
{
	SpinnerStart();
	string local_skin_path;
	bool status_code;
	
	if(file_chooser->get_filename() == "")
	{
		local_skin_path = comboboxthemes->get_active_text();
		if(local_skin_path == "Stock")
		{
			if(settings.RevertSkin())
			{
				notify->set_markup("Theme was successfully reverted.");
				notify->override_color(Gdk::RGBA("#0a0"));
			}
			else
			{
				notify->set_markup("Steam Skin Manager could not revert the theme.");
				notify->override_color(Gdk::RGBA("#a00"));
				cerr << "Can't revert to the default due to a missing execution shell script." << endl;
			}
		}
	}
	else
		local_skin_path = file_chooser->get_filename();
	
	if(file_chooser->get_filename() == "")
	{
		if(use_decorations->get_active())
			local_skin_path += "/no-buttons"; // no-buttons are skins relying on native window decorations
		else
			local_skin_path += "/with-buttons";
		
		status_code = settings.SetInstalledSkin(local_skin_path);
	}
	else {
		status_code = settings.SetSkin(local_skin_path);
	}
	
    if(status_code != -1 && status_code != 127)
    {
        notify->set_markup("Theme was successfully applied.");
        notify->override_color(Gdk::RGBA("#0a0"));
	}
    else
    {
        notify->set_markup("Steam Skin Manager could not set the theme.");
        notify->override_color(Gdk::RGBA("#a00"));
	}
	
    SpinnerStop();
}

void MainWindow::PreviewTheme(bool preinstalled)
{
	SpinnerStart();
	string skin_name, skin_path;
	
	preview_description->set_line_wrap(true);
	preview_image->override_background_color(Gdk::RGBA("#333"));
	
	if(preinstalled)
	{
		skin_name = comboboxthemes->get_active_text();
		if(skin_name == "Stock")
		{
			skin_path = "stock";
			SetPreviewImage(settings.GetSystemPath() + "/images/stock.png", "<i>The is the default and official Steam theme.</i>");
			SpinnerStop();
			return;
		}
		else
			skin_path = settings.GetPath("theme") + skin_name;
	}
	else
		skin_path = file_chooser->get_filename();
	
	// Preview ... description
	if(skin_name != "")
		SetPreviewImage(skin_path + "/preview.png", settings.GetFileContent(skin_path + "/description.txt"));
	else
		SetPreviewImage(settings.GetSystemPath() + "/images/image_missing.png", "No description available for this theme.");
	SpinnerStop();
}

void MainWindow::SetPreviewImage(string path, string description)
{
	Glib::RefPtr<Gdk::Pixbuf> tmp_image = Gdk::Pixbuf::create_from_file(path);
	tmp_image = tmp_image->scale_simple(500, 277, Gdk::INTERP_BILINEAR);
	preview_image->set(tmp_image);
	
	if(description != "")
	{
		preview_description->set_markup(description);
	}
}

void MainWindow::ShowTips()
{
	notify->set_markup(settings.GetTip(true));
	
	prev_tip->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::PreviousTip));
	next_tip->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::NextTip));
}

void MainWindow::NextTip()
{
	notify->set_markup(settings.GetTip(true));
	notify->unset_color();
}

void MainWindow::PreviousTip()
{
	notify->set_markup(settings.GetTip(false));
	notify->unset_color();
}

void MainWindow::LaunchSteam()
{
	SpinnerStart();
	cout << "Launching steam ..." << endl;
	Steam steam(settings);
	steam.Run(use_decorations->get_active());
}

MainWindow::~MainWindow()
{
	delete manualeditor;
	cout << "Bye Bye " << getenv("USER") << " ..." << endl;
}

void MainWindow::ListAvaialbleThemes(string path)
{
	vector<string> test = settings.GetListOfAvaialbleFolders(path);
	string theme = settings.GetSystemTheme();
	cout << "current theme is: " << theme << endl;
	
	// Combobox
	builder->get_widget("comboboxthemes", comboboxthemes);
			
	int i = 1;
	comboboxthemes->insert(0, "Stock");
	for(vector<string>::iterator it = test.begin(); it != test.end(); ++it)
	{
		comboboxthemes->insert(i, (*it));
		
		if(string(*it) == theme)
			comboboxthemes->set_active(i);
		
		i++;
	}
	
	PreviewTheme(false);		
}

void MainWindow::SpinnerStart()
{
	spinner->start();
}

void MainWindow::SpinnerStop()
{
	spinner->stop();
}

void MainWindow::CreateLauncher()
{
	settings.CreateLauncher();
}
