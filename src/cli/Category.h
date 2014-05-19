#ifndef CATEGORY
#define CATEGORY

class Category {
	protected:
		string title;
		string underline;
	public:
		void SetTitle(string gained_title);
		virtual void Print(ostream& os) const;
        virtual void SkipLines(ostream& os, int n) const;
};

ostream& operator << (ostream& os, const Category &category);

#endif /* CATEGORY */
