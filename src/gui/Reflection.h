#ifndef REFLECTION
#define REFLECTION


class Reflection {
	private:
		const Settings & settings;
		
		Glib::RefPtr<Gtk::Builder> builder;
		Glib::RefPtr<Gtk::TextBuffer> TextBuffer;
		Gtk::Button* refpreview;
		Gtk::Button* refsave;
		Gtk::Viewport* refbg;
		Gtk::Entry* name;
		Gtk::Entry* description;
		Gtk::ColorButton* color1;
		Gtk::ColorButton* color2;
		Gtk::ColorButton* color3;
		Gtk::ColorButton* color4;
		Gtk::ColorButton* color5;
		Gtk::ColorButton* color6;
		Gtk::ColorButton* color7;
		Gtk::ColorButton* color8;
		Gtk::FontButton* fontbutton;
		Gtk::CheckButton* ref_check_no_decorations;
		Gtk::CheckButton* ref_check_with_decorations;
		Gtk::Label* notify;
		
		string load_path;
		string save_path;
		string content;
				
		Gtk::TextView* view;
		
		void Preview();
		void GetRGBColor(Gdk::RGBA rgba, int &R, int &G, int &B);
		void GetColor(Gtk::ColorButton* cbtn, int &R, int &G, int &B);
		void InvertColor(int &R, int &G, int &B);
		string ConstructReplacement(int R, int G, int B, int transparency = 255);
		void FillTemplate();
		void Fill(string match, string replace);
		void SaveTheme();
		
	public:
		Reflection(Glib::RefPtr<Gtk::Builder> &builder, const Settings & settings);
};

#endif /* REFLECTION */
