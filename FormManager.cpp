#include "FormManager.hpp"
#include "Application.hpp"
#include "Controls\Form.hpp"
#include "Drawing\IRenderer.hpp"
#include "Event\IEvent.hpp"
#include "Misc\Exceptions.hpp"

namespace OSHGui
{
	const std::shared_ptr<Form> FormManager::GetForeMost() const
	{
		if (forms.size() != 0)
		{
			return forms[forms.size() - 1].form;
		}
		return 0;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Form>& FormManager::operator [] (int index) const
	{
		if (index < 0 || index >= (int)forms.size())
		{
			throw Misc::ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
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
		if (form == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (forms.empty())
		{
			throw Misc::InvalidOperationException(L"FormList is empty.", __WFILE__, __LINE__);
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
			throw Misc::InvalidOperationException(L"'form' is not registered.", __WFILE__, __LINE__);
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
		RegisterForm(form, 0);
	}
	//---------------------------------------------------------------------------
	void FormManager::RegisterForm(const std::shared_ptr<Form> &form, std::function<void()> closeFunction)
	{
		if (form == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}
		
		if (IsRegistered(form))
		{
			return;
		}

		FormInfo info = { form, closeFunction };
		forms.push_back(info);
	}
	//---------------------------------------------------------------------------
	void FormManager::UnregisterForm(const std::shared_ptr<Form> &form)
	{
		if (form == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (mainForm == form)
		{
			Application::Instance()->Disable();
		}

		for (std::vector<FormInfo>::iterator it = forms.begin(); it != forms.end(); ++it)
		{
			FormInfo info = *it;
			if (info.form == form)
			{
				forms.erase(it);
				if (info.closeFunction != 0)
				{
					info.closeFunction();
				}
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	bool FormManager::ForwardEventToForms(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (forms.size() > 0)
		{
			std::shared_ptr<Form> foreMost = GetForeMost();
			if (foreMost != 0 && foreMost->IsModal())
			{
				return foreMost->ProcessEvent(event);
			}
			
			for (std::vector<FormInfo>::reverse_iterator it = forms.rbegin(); it != forms.rend(); ++it)
			{
				FormInfo info = *it;
				if (info.form->ProcessEvent(event) == true)
				{
					if (info.form != foreMost)
					{
						BringToFront(info.form);
					}

					return true;
				}
			}
		}
		
		return false;
	}
	//---------------------------------------------------------------------------
	void FormManager::RenderForms(Drawing::IRenderer *renderer)
	{		
		if (forms.size() > 0)
		{
			for (std::vector<FormInfo>::reverse_iterator it = ++forms.rbegin(); it != forms.rend(); ++it)
			{
				(*it).form->Render(renderer);
			}

			GetForeMost()->Render(renderer);
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
		it++;
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