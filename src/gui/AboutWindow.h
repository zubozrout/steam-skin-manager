#ifndef ABOUT_WINDOW
#define ABOUT_WINDOW


class AboutWindow {
	private:
		Gtk::AboutDialog* window;
		const Settings & settings;
	public:
		AboutWindow(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings);

};

#endif /* ABOUT_WINDOW */
