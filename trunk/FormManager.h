#ifndef OSHGUI_FORMMANAGER_H_
#define OSHGUI_FORMMANAGER_H_

#include <list>

namespace OSHGui
{
	class Form;

	class FormManager
	{
	public:
		Form* GetForeMost() const;

		Form* operator [] (int index) const;

		void RegisterWindow(Form *form);
		void Remove(Form *form);
		void BringToFront(Form *form);

	private:
		std::list<Form*> formList;
	};
}

#endif