#ifndef WELCOME_SCREEN
#define WELCOME_SCREEN

class WelcomeScreen : public Category {
	protected:
		string user;
		
	public:
		WelcomeScreen();
	
		virtual void Print(ostream& os) const;
};

#endif /* WELCOME_SCREEN */
