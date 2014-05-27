#include <gtkmm.h>

#include "AboutWindow.h"

using namespace std;

AboutWindow::AboutWindow(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings): settings(linked_settings)
{	
	builder->get_widget("aboutdialog", window);
	window->set_version(settings.GetSoftwareVersion());
	window->set_program_name(settings.GetApplicationName());
	
	try
	{
		Glib::RefPtr<Gdk::Pixbuf> logo = Gdk::Pixbuf::create_from_file(settings.GetSystemPath() + "images/logo.svg");
		window->set_logo(logo);
	}
	catch(...)
	{
		cerr << "Missing SSM logo." << endl;
	}
	
	gint result = window->run();
	switch (result)
	{
        case GTK_RESPONSE_DELETE_EVENT:
        break;
    }

    window->hide();
}
