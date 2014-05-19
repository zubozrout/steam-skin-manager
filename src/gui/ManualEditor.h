#ifndef MANUAL_EDITOR
#define MANUAL_EDITOR


class ManualEditor {
	private:
		const Settings & settings;
		Glib::RefPtr<Gtk::Builder> builder;
		
		// Manual edit
		Gtk::TextView* manual_editor;
		Gtk::Button* manual_save;
		Gtk::Button* manual_refresh;
		Gtk::Scale* scale;
		Glib::RefPtr<Gtk::TextBuffer> TextBuffer;
		string backup;
		string path;
		int size;
		
		void ManualFirstLoad();
		void ManualRefresh();
		void ManualSave();
		void Size();
		
	public:
		ManualEditor(Glib::RefPtr<Gtk::Builder> &builder, const Settings & settings);
};

#endif /* MANUAL_EDITOR */
