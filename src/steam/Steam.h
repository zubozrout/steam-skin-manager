#ifndef STEAM
#define STEAM

class Steam {
	private:
		const Settings & settings;
		
		static void reap_child(int sig);
		static void Done();

	public:
		Steam(const Settings & linked_settings);
		void Run(bool native_decorations);
};

#endif /* STEAM */
