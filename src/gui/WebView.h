#ifndef WEB_VIEW
#define WEB_VIEW

extern "C"
{
	static void load_finished_trampoline(WebKitWebView *webview, WebKitWebFrame *frame, gpointer userdata);
}

class WebView {
	private:
		friend void load_finished_trampoline(WebKitWebView *webview, WebKitWebFrame *frame, gpointer userdata);

		Glib::RefPtr<Gtk::Builder> builder;
		const Settings & settings;
		
		// Help (web) browser
		WebKitWebView* webview;
		WebKitLoadStatus status;
		Gtk::ScrolledWindow* webview_help;
		Gtk::Button* help_home_button;
		Gtk::Button* help_search_button;
		Gtk::Button* help_forward_button;
		Gtk::Button* help_back_button;
		Gtk::Entry* help_url_entry;
		
		//_WRAP_SIGNAL(void load_finished(const Glib::RefPtr<WebFrame>& webview), "load-finished", ShowUrl());
		
		void HelpGoHome();
		void HelpGoBack();
		void HelpGoForward();
		void HelpGo();
		void ShowUrl();
		void LoadStatus();
		void ModifyUrl();
		
		/*
		static void destroy_cb(GtkWidget* widget, gpointer data);
		static void load_finished_cb(WebKitWebView *web_view, WebKitWebFrame *web_frame, gpointer data);
		static void load_status_cb(GObject* object, GParamSpec* pspec, gpointer data);
		*/
		
	public:
		WebView(Glib::RefPtr<Gtk::Builder> &builder, const Settings & linked_settings);
};

#endif /* WEB_VIEW */
