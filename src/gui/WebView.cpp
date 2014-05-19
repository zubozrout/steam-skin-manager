#include <gtkmm.h>
#include <webkit/webkit.h>

#include "WebView.h"

using namespace std;

extern "C"
{
	static void load_finished_trampoline(WebKitWebView *webview, WebKitWebFrame *frame, gpointer userdata)
 	{
		WebView *self = static_cast<WebView*>(userdata);
		self->ShowUrl();		
	}
}

WebView::WebView(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings): settings(linked_settings)
{
	// Webview
	builder->get_widget("help_back_button", help_back_button);
	builder->get_widget("help_forward_button", help_forward_button);
	builder->get_widget("help_home_button", help_home_button);
	builder->get_widget("help_search_button", help_search_button);
	builder->get_widget("webview_help", webview_help);
	builder->get_widget("help_url_entry", help_url_entry);
	
	// Webview Tooblar
	help_back_button->signal_clicked().connect(sigc::mem_fun(*this, &WebView::HelpGoBack));
	help_forward_button->signal_clicked().connect(sigc::mem_fun(*this, &WebView::HelpGoForward));
	help_home_button->signal_clicked().connect(sigc::mem_fun(*this, &WebView::HelpGoHome));
	help_search_button->signal_clicked().connect(sigc::mem_fun(*this, &WebView::HelpGo));
	// Webview ... (help)
	webview = WEBKIT_WEB_VIEW(webkit_web_view_new());
	help_url_entry->signal_activate().connect(sigc::mem_fun(*this, &WebView::HelpGo));
	
	webview_help->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	g_object_ref_sink(G_OBJECT(webview));
	gtk_container_add(GTK_CONTAINER(webview_help->gobj()), GTK_WIDGET(webview));
	gtk_widget_show_all(GTK_WIDGET(webview));
	
	/* Register a callback that gets invoked each time that a page is finished downloading */
    //g_signal_connect(webview, "load-finished", G_CALLBACK(load_finished_cb), NULL);

    /* Register a callback that gets invoked each time that the load status changes */
    //g_object_connect(webview, "signal::notify::load-status", G_CALLBACK(load_status_cb), NULL);
    
	g_signal_connect(webview, "load_finished", G_CALLBACK(load_finished_trampoline), this);
	
	HelpGoHome();
}

/*
static void WebView::destroy_cb(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
  }

static void WebView::load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame, gpointer data) {
      printf("Finished downloading %s\n", webkit_web_view_get_uri(webview));
  }

  static void WebView::load_status_cb(GObject* object, GParamSpec* pspec, gpointer data) {
      WebKitWebView *webview;
      WebKitLoadStatus status;
      const gchar *uri;

      webview = WEBKIT_WEB_VIEW(object);
      status = webkit_web_view_get_load_status(webview);
      uri = webkit_web_view_get_uri(webview);

      switch (status) {
      case WEBKIT_LOAD_PROVISIONAL:
          printf("Load provisional: %s\n", uri);
          break;
      case WEBKIT_LOAD_COMMITTED:
          printf("Load commited: %s\n", uri);
          break;
      case WEBKIT_LOAD_FIRST_VISUALLY_NON_EMPTY_LAYOUT:
          printf("Load first visually non empty layout: %s\n", uri);
          break;
      case WEBKIT_LOAD_FINISHED:
          printf("Load finished: %s\n", uri);
          break;
      default:
          g_assert_not_reached();
      }
  }
*/

void WebView::HelpGoHome()
{
	string uri = settings.Key("help_html");
	if(uri[4] == ':' && uri[5] == '/'  && uri[6] == '/')
	{
		uri = settings.Key("help_html");
		cout << "Loading http url address." << endl;
	}
	else if(uri[5] == ':' && uri[6] == '/'  && uri[7] == '/')
	{
		uri = settings.Key("help_html");
		cout << "Loading https url address." << endl;
	}
	else
		uri = "file://" + settings.get_working_path() + "/" + settings.Key("data_path") + settings.Key("help_html");
	
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), (uri).c_str());
}

void WebView::HelpGoBack()
{
	webkit_web_view_go_back(webview);
}

void WebView::HelpGoForward()
{
	webkit_web_view_go_forward(webview);
}
void WebView::HelpGo()
{
	ModifyUrl();
	string url = help_url_entry->get_text();
	webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), url.c_str());
	help_url_entry->set_text("loading ... " + url);
}

void WebView::ShowUrl()
{
	//const char *title = webkit_web_view_get_title(webview);
	const char *title = webkit_web_view_get_uri(webview);
	if (title != NULL)
		help_url_entry->set_text(title);
}

void WebView::LoadStatus()
{
		status = webkit_web_view_get_load_status(webview);
		
		if (status == WEBKIT_LOAD_FINISHED)
		{
			ShowUrl();
			cout << "finished" << endl;
		}
}

void WebView::ModifyUrl()
{
	string current_text = help_url_entry->get_text();
	if(current_text.find("://") == string::npos)
		current_text = "http://" + current_text;
	help_url_entry->set_text(current_text);
}
