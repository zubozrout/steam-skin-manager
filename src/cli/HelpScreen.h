#ifndef HELP_SCREEN
#define HELP_SCREEN

class Help : public Category {
	protected:
		string help;
		
	public:
		Help();
		virtual void Print(ostream& os) const;
};

#endif /* HELP_SCREEN */
