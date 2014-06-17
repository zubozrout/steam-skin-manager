#include "Reflection.h"

using namespace std;

Reflection::Reflection(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings): settings(linked_settings)
{
	builder->get_widget("notification_label", notify);
	builder->get_widget("reflection_view", view);
	builder->get_widget("refbutton_preview", refpreview);
	builder->get_widget("refbutton_save", refsave);
	builder->get_widget("refbg", refbg);
	builder->get_widget("ref_name", name);
	builder->get_widget("ref_desc_text", description);
	builder->get_widget("colorbutton1", color1);
	builder->get_widget("colorbutton2", color2);
	builder->get_widget("colorbutton3", color3);
	builder->get_widget("colorbutton4", color4);
	builder->get_widget("colorbutton5", color5);
	builder->get_widget("colorbutton6", color6);
	builder->get_widget("colorbutton7", color7);
	builder->get_widget("colorbutton8", color8);
	builder->get_widget("fontbutton", fontbutton);
	builder->get_widget("ref_check_no_decorations", ref_check_no_decorations);
	builder->get_widget("ref_check_with_decorations", ref_check_with_decorations);
			
	refpreview->signal_clicked().connect(sigc::mem_fun(*this, &Reflection::FillTemplate));
	refsave->signal_clicked().connect(sigc::mem_fun(*this, &Reflection::SaveTheme));
	
	load_path = settings.get_working_path() + "share/steam-skin-manager/doc/";
	save_path = settings.GetLocalPath() + "themes/";
	TextBuffer = Gtk::TextBuffer::create();
	Preview();
}

void Reflection::Preview() {
	try {
		if(content.empty()) {
			content = settings.GetFileContent(load_path + "steam.styles");
		}
		TextBuffer->set_text(content); 
		view->set_buffer(TextBuffer);
	}
	catch(...) {
		cerr << "Could not load reflection template, file is missing!" << endl;
	}
}

void Reflection::GetRGBColor(Gdk::RGBA rgba, int &R, int &G, int &B) {
	R = rgba.get_red_u()/257;
	G = rgba.get_green_u()/257;
	B = rgba.get_blue_u()/257;
}

void Reflection::GetColor(Gtk::ColorButton* cbtn, int &R, int &G, int &B) {
	GetRGBColor(cbtn->get_rgba(), R, G, B);
}

void Reflection::InvertColor(int &R, int &G, int &B) {
	R = 255 - R;
	G = 255 - G;
	B = 255 - B;
}

string Reflection::ConstructReplacement(int R, int G, int B, int transparency) {
	string str = "\"";
	str += to_string(R);
	str += " ";
	str += to_string(G);
	str += " ";
	str += to_string(B);
	str += " ";
	str += to_string(transparency);
	str += "\"";
	return str;
}

void Reflection::FillTemplate() {
	content = settings.GetFileContent(load_path + "steam.styles");
	int R, G, B;
	string replacement;
	
	GetColor(color1, R, G, B);
	replacement = ConstructReplacement(R, G, B);
	Fill("\"$BLACK\"", replacement);
	Fill("\"$BACKGROUND\"", replacement);
	Fill("\"$MENUBACKGROUND\"", replacement);
	Fill("\"$GRADIENTBG\"", replacement);
	Fill("\"$GRADIENTMENUBG\"", replacement);
	
	InvertColor(R, G, B);
	Fill("\"$WHITE\"", ConstructReplacement(R, G, B));
	
	GetColor(color2, R, G, B);
	replacement = ConstructReplacement(R, G, B);
	Fill("\"$ALMOSTBLACK\"", replacement);
	Fill("\"$DIVIDER\"", replacement);
	Fill("\"$SUPERDIVIDER\"", replacement);
	
	GetColor(color4, R, G, B);
	Fill("\"$TEXTCOLOR\"", ConstructReplacement(R, G, B));
	
	GetColor(color5, R, G, B);
	Fill("\"$TEXTCOLOR2\"", ConstructReplacement(R, G, B));
	
	GetColor(color6, R, G, B);
	Fill("\"$TEXTDISABLED\"", ConstructReplacement(R, G, B));
	
	GetColor(color7, R, G, B);
	Fill("\"$TEXTHOVER\"", ConstructReplacement(R, G, B));
	
	GetColor(color8, R, G, B);
	Fill("\"$FOCUS\"", ConstructReplacement(R, G, B));
	
	Fill("\"$FONT\"", fontbutton->get_font_name());
}

void Reflection::Fill(string match, string replace) {
	cout << match << " :: " << replace << endl;
	while(content.find(match) != string::npos) {
		content.replace(content.find(match), match.length(), replace);
	}
	Preview();
}

void Reflection::SaveTheme() {
	bool no_decors = ref_check_no_decorations->get_active();
	bool decors = ref_check_with_decorations->get_active();
	string skin_name = name->get_text();
	if(skin_name == "") {
		notify->set_markup(_("<b>Theme could not be saved without a name!</b>"));
		notify->override_color(Gdk::RGBA("#a33"));
		return;
	}
	string skin_desc = description->get_text();
	mkdir((save_path).c_str(), 0751);
	
	mkdir((save_path + skin_name + "/").c_str(), 0751);
	if(decors) {
		mkdir((save_path + skin_name + "/no-buttons/").c_str(), 0751);
		mkdir((save_path + skin_name + "/no-buttons/Graphics").c_str(), 0751);
		mkdir((save_path + skin_name + "/no-buttons/resource/").c_str(), 0751);
		mkdir((save_path + skin_name + "/no-buttons/resource/styles/").c_str(), 0751);
		system(("cp -rf '" + load_path + "no-buttons/* " + save_path + skin_name + "/no-buttons/'").c_str());
		
		ofstream out(save_path + skin_name + "/no-buttons/resource/styles/" + "steam.styles");
		out << TextBuffer->get_text();
		out.close();
	}
	
	if(no_decors) {
		mkdir((save_path + skin_name + "/with-buttons/").c_str(), 0751);
		mkdir((save_path + skin_name + "/with-buttons/Graphics").c_str(), 0751);
		mkdir((save_path + skin_name + "/with-buttons/resource/").c_str(), 0751);
		mkdir((save_path + skin_name + "/with-buttons/resource/styles/").c_str(), 0751);
		
		ofstream out(save_path + skin_name + "/with-buttons/resource/styles/" + "steam.styles");
		out << TextBuffer->get_text();
		out.close();
	}
	
    if(!no_decors && !decors) {
		mkdir((save_path + skin_name + "/").c_str(), 0751);
		mkdir((save_path + skin_name + "/Graphics").c_str(), 0751);
		mkdir((save_path + skin_name + "/resource/").c_str(), 0751);
		mkdir((save_path + skin_name + "/resource/styles/").c_str(), 0751);
		
		ofstream out(save_path + skin_name + "/resource/styles/" + "steam.styles");
		out << TextBuffer->get_text();
		out.close();
	}
	
	ofstream out(save_path + skin_name + "/description.txt");
	out << skin_desc;
	out.close();
}
