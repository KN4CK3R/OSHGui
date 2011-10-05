#include "FormManager.h"
#include "Form.h"
#include "Exceptions.h"

namespace OSHGui
{
	Form* FormManager::GetForeMost() const
	{
		if (formList.size() != 0)
		{
			return *formList.begin();
		}
		return 0;
	}

	Form* FormManager::operator [] (int index) const
	{
		if (index < 0 || index >= (int)formList.size())
		{
			throw ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}

		std::list<Form*>::const_iterator it = formList.begin();
		for (int i = 0; i < index; ++i, ++it);
		return *it;
	}

	void FormManager::BringToFront(Form *form)
	{
		if (form == 0)
		{
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (formList.empty())
		{
			throw InvalidOperationException(L"FormList is empty.", __WFILE__, __LINE__);
		}

		if (*formList.begin() == form)
		{
			return;
		}

		formList.remove(form);
		formList.push_front(form);
	}

	void FormManager::RegisterWindow(Form *form)
	{
		if (form == 0)
		{
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		formList.push_front(form);
	}

	void FormManager::Remove(Form *form)
	{
		if (form == 0)
		{
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (formList.empty())
		{
			throw InvalidOperationException(L"FormList is empty.", __WFILE__, __LINE__);
		}

		formList.remove(form);
	}
}