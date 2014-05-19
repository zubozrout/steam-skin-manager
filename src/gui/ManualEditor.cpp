#include "ManualEditor.h"

using namespace std;

ManualEditor::ManualEditor(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings): settings(linked_settings)
{
	// Editor
	builder->get_widget("manual_text", manual_editor);
	builder->get_widget("manual_save", manual_save);
	builder->get_widget("manual_reload", manual_refresh);
	builder->get_widget("scale", scale);
	
	manual_save->signal_clicked().connect(sigc::mem_fun(*this, &ManualEditor::ManualSave));
	manual_refresh->signal_clicked().connect(sigc::mem_fun(*this, &ManualEditor::ManualRefresh));
	scale->signal_value_changed().connect(sigc::mem_fun(*this, &ManualEditor::Size));
	
	TextBuffer = Gtk::TextBuffer::create();
	
	manual_editor->override_font(Pango::FontDescription("monospace"));
	//manual_editor->override_color(Gdk::RGBA("#fff"));
	path = settings.GetHomePath() + "/.steam/registry.vdf";
	ManualFirstLoad();
	
	// Scaling
	scale->set_digits(0);
	scale->set_range(6, 24);
	scale->set_value(10);
	Size();
}

void ManualEditor::ManualSave()
{
	Glib::RefPtr<Gtk::TextBuffer> TextBuffer = Gtk::TextBuffer::create();
	TextBuffer = manual_editor->get_buffer();
	string manual_content = TextBuffer->get_text(true);
	cout << "Saving registry.vdf to " + path + " ..." << endl;
	
	ofstream out;
	out.open((path).c_str());
	out << manual_content;
	out.close();
}

void ManualEditor::ManualFirstLoad()
{
	try
	{
		backup = settings.GetFileContent(path);		
		manual_refresh->set_sensitive(false);
		
		TextBuffer->set_text(backup); 
		manual_editor->set_buffer(TextBuffer);
	}
	catch (...)
	{
		cerr << "An error occured while trying to manipulate with the text buffer;" << endl;
	}
}

void ManualEditor::ManualRefresh()
{
	try
	{
		TextBuffer->set_text(backup);
	}
	catch (...)
	{
		cerr << "An error occured while trying to manipulate with the text buffer;" << endl;
	}
}

void ManualEditor::Size()
{
	size = scale->get_value();
	manual_editor->override_font(Pango::FontDescription(to_string(size) + "px"));
}
