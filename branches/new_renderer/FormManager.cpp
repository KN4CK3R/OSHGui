/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "FormManager.hpp"
#include "Application.hpp"
#include "Controls/Form.hpp"
#include "Misc/Exceptions.hpp"

namespace OSHGui
{
	const std::shared_ptr<Form> FormManager::GetForeMost() const
	{
		if (!forms.empty())
		{
			return forms[forms.size() - 1].form;
		}
		return nullptr;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Form>& FormManager::operator [] (int index) const
	{
		if (index < 0 || index >= (int)forms.size())
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentOutOfRangeException("index");
			#endif
		}

		return forms[index].form;
	}
	//---------------------------------------------------------------------------
	int FormManager::GetFormCount() const
	{
		return (int)forms.size();
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Form>& FormManager::GetMainForm() const
	{
		return mainForm;
	}
	//---------------------------------------------------------------------------
	void FormManager::BringToFront(const std::shared_ptr<Form> &form)
	{
		if (form == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("form");
			#endif
			return;
		}

		if (forms.empty())
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::InvalidOperationException("FormList is empty.");
			#endif
			return;
		}

		if (forms[forms.size() - 1].form == form) //last form in vector is front form
		{
			return;
		}
		
		FormInfo info;
		bool isRegistered = false;
		for (auto it = forms.begin(); it != forms.end(); ++it)
		{
			info = *it;
			if (info.form == form)
			{
				forms.erase(it);
				isRegistered = true;
				break;
			}
		}

		if (!isRegistered)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::InvalidOperationException("'form' is not registered.");
			#endif
			return;
		}
		
		forms.push_back(info);
	}
	//---------------------------------------------------------------------------
	bool FormManager::IsRegistered(const std::shared_ptr<Form> &form)
	{
		for (std::vector<FormInfo>::iterator it = forms.begin(); it != forms.end(); ++it)
		{
			FormInfo &info = *it;
			if (info.form == form)
			{
				return true;
			}
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	void FormManager::RegisterMainForm(const std::shared_ptr<Form> &mainForm)
	{
		this->mainForm = mainForm;
		RegisterForm(mainForm);
	}
	//---------------------------------------------------------------------------
	void FormManager::RegisterForm(const std::shared_ptr<Form> &form)
	{
		RegisterForm(form, nullptr);
	}
	//---------------------------------------------------------------------------
	void FormManager::RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction)
	{
		if (form == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("form");
			#endif
			return;
		}
		
		if (IsRegistered(form))
		{
			return;
		}

		FormInfo info = { form, closeFunction, false };
		forms.push_back(info);
	}
	//---------------------------------------------------------------------------
	void FormManager::UnregisterForm(const std::shared_ptr<Form> &form)
	{
		if (form == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("form");
			#endif
			return;
		}

		for (std::vector<FormInfo>::iterator it = forms.begin(); it != forms.end(); ++it)
		{
			FormInfo &info = *it;
			if (info.form == form)
			{
				if (info.closeFunction)
				{
					info.closeFunction();
				}
				if (info.form != mainForm)
				{
					info.remove = true;
				}
				else
				{
					Application::Instance()->Disable();
				}
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	void FormManager::RemoveUnregisteredForms()
	{
		for (std::vector<FormInfo>::iterator it = forms.begin(); it != forms.end();)
		{
			if ((*it).remove)
			{
				it = forms.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	//---------------------------------------------------------------------------
	FormManager::FormIterator FormManager::GetEnumerator()
	{
		return FormIterator(*this);
	}
	//---------------------------------------------------------------------------
	FormManager::FormIterator::FormIterator(FormManager &fm)
	{
		it = fm.forms.rbegin();
		end = fm.forms.rend();
	}
	//---------------------------------------------------------------------------
	void FormManager::FormIterator::operator++()
	{
		++it;
	}
	//---------------------------------------------------------------------------
	bool FormManager::FormIterator::operator()()
	{
		return it != end;
	}
	//---------------------------------------------------------------------------
	std::shared_ptr<Form>& FormManager::FormIterator::operator*()
	{
		return (*it).form;
	}
	//---------------------------------------------------------------------------
}