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
	ListAvaialbleThemes(settings.get_working_path() + "/" + settings.GetPath("theme"));
	ListAvaialbleThemes(settings.Key("old_theme_path")); /* Steam Skin Manager (3.x) and older backward compatibility */
	
	// Apply theme button
	apply->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::ApplyTheme));
	
	// File chooser
	comboboxthemes->signal_changed().connect(sigc::group(sigc::mem_fun(*this, &MainWindow::PreviewTheme), true));
	file_chooser->signal_file_set().connect(sigc::group(sigc::mem_fun(*this, &MainWindow::PreviewTheme), false));
	
	header->set_markup("Hello " + settings.GetFullUserName() + ", Welcome to " + settings.GetApplicationName());
	header->override_font(Pango::FontDescription("18px"));
	
	// Launch Steam button icon 
	Glib::RefPtr<Gdk::Pixbuf> pix = Gdk::Pixbuf::create_from_file(settings.get_working_path() + "/share/steam-skin-manager/images/logo.svg");
	int iconW, iconH;
	Gtk::IconSize::lookup(launch_steam_from_toolbar->get_icon_size(), iconW, iconH);
	logo->set(pix->scale_simple(iconW + 5, iconH + 5, Gdk::INTERP_BILINEAR));
	
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
	string selected_skin;
	bool status_code;
	
	if(file_chooser->get_filename() == "")
	{
		selected_skin = comboboxthemes->get_active_text();
		if(selected_skin == "Stock")
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
		else {
			selected_skin = bundledSkins[comboboxthemes->get_active_row_number()];
		}
	}
	else
		selected_skin = file_chooser->get_filename();
	
	if(file_chooser->get_filename() == "")
	{
		if(use_decorations->get_active())
			selected_skin += "/no-buttons"; // no-buttons are skins relying on native window decorations
		else
			selected_skin += "/with-buttons";
		
		status_code = settings.SetSkin(selected_skin);
	}
	else {
		status_code = settings.SetSkin(selected_skin);
	}
	
    if(status_code != -1 && status_code != 127)
    {
        notify->set_markup("<b>Theme was applied.</b>");
        notify->override_color(Gdk::RGBA("#3a3"));
	}
    else
    {
        notify->set_markup("<b>Steam Skin Manager could not set the theme.</b>");
        notify->override_color(Gdk::RGBA("#a33"));
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
			skin_path = bundledSkins[comboboxthemes->get_active_row_number()];
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
		
	// Combobox
	builder->get_widget("comboboxthemes", comboboxthemes);
				
	int i = bundledSkins.size();
	if(bundledSkins.empty() || bundledSkins.front() != "Stock") {	
		comboboxthemes->insert(0, "Stock");
		bundledSkins.push_back("Stock");
		i++;
	}
	
	for(vector<string>::iterator it = test.begin(); it != test.end(); ++it) {
		comboboxthemes->insert(i, (*it));
		bundledSkins.push_back(path + (*it));
		cout << path + (*it) << endl;
		
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
