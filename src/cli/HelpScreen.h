#ifndef HELP_SCREEN
#define HELP_SCREEN

class Help : public Category {
	private:
		const Settings & settings;
		string filepath;
		
	protected:
		string help;
		
	public:
		Help(const Settings & linked_settings);
		void GetPath();
		virtual void Print(ostream& os) const;
};

#endif /* HELP_SCREEN */
