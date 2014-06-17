#include "ManualEditor.h"

using namespace std;

ManualEditor::ManualEditor(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings): settings(linked_settings)
{
	// Editor
	builder->get_widget("manual_text", manual_editor);
	builder->get_widget("manual_save", manual_save);
	builder->get_widget("manual_revert", manual_revert);
	builder->get_widget("manual_reload", manual_reload);
	builder->get_widget("scale", scale);
	
	manual_save->signal_clicked().connect(sigc::mem_fun(*this, &ManualEditor::Save));
	manual_revert->signal_clicked().connect(sigc::mem_fun(*this, &ManualEditor::Revert));
	manual_reload->signal_clicked().connect(sigc::mem_fun(*this, &ManualEditor::FirstLoad));
	scale->signal_value_changed().connect(sigc::mem_fun(*this, &ManualEditor::Size));
	
	TextBuffer = Gtk::TextBuffer::create();
	TextBuffer->signal_changed().connect(sigc::mem_fun(*this, &ManualEditor::Editing));
	
	manual_editor->override_font(Pango::FontDescription("monospace"));
	path = settings.GetHomePath() + "/.steam/registry.vdf";
	FirstLoad();
	
	// Scaling
	scale->set_digits(0);
	scale->set_range(6, 24);
	scale->set_value(10);
	scale->set_tooltip_markup(_("Scale font"));
	Size();
}

void ManualEditor::Save()
{
	//Glib::RefPtr<Gtk::TextBuffer> TextBuffer = Gtk::TextBuffer::create();
	TextBuffer = manual_editor->get_buffer();
	string manual_content = TextBuffer->get_text(true);
	cout << "Saving registry.vdf to " + path + " ..." << endl;
	
	ofstream out;
	out.open((path).c_str());
	out << manual_content;
	out.close();
}

void ManualEditor::FirstLoad()
{
	try
	{
		backup = settings.GetFileContent(path);		
		manual_revert->set_sensitive(false);
		
		TextBuffer->set_text(backup); 
		manual_editor->set_buffer(TextBuffer);
	}
	catch (...)
	{
		cerr << "An error occured while trying to manipulate with the text buffer;" << endl;
	}
}

void ManualEditor::Editing()
{
	manual_revert->set_sensitive(true);
}

void ManualEditor::Revert()
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
