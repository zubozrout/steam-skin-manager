#ifndef STEAM
#define STEAM

class Steam {
	private:
		const Settings & settings;

	public:
		Steam(const Settings & linked_settings);
		void Run(bool native_decorations);
};

#endif /* STEAM */
