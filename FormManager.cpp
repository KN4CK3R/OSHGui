#include "FormManager.h"
#include "Form.h"
#include "Misc\Exceptions.h"

namespace OSHGui
{
	const std::shared_ptr<Form>& FormManager::GetForeMost() const
	{
		if (formList.size() != 0)
		{
			return (*formList.begin()).form;
		}
		return 0;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Form>& FormManager::operator [] (int index) const
	{
		if (index < 0 || index >= (int)formList.size())
		{
			throw Misc::ArgumentOutOfRangeException(L"index", __WFILE__, __LINE__);
		}

		std::list<Form*>::const_iterator it = formList.begin();
		for (int i = 0; i < index; ++i, ++it);
		return (*it).form;
	}
	//---------------------------------------------------------------------------
	void FormManager::BringToFront(const std::shared_ptr<Form> &form)
	{
		if (form == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (formList.empty())
		{
			throw Misc::InvalidOperationException(L"FormList is empty.", __WFILE__, __LINE__);
		}

		if ((*formList.begin()).form == form)
		{
			return;
		}
		
		FormInfo info;
		bool isRegistered = false;
		for (auto it = formList.begin(); it != formList.end(); ++it)
		{
			info = *it;
			if (info.form == form)
			{
				formList.erase(it);
				isRegistered = true;
				break;
			}
		}

		if (!isRegistered)
		{
			throw Misc::InvalidOperationException(L"'form' is not registered.", __WFILE__, __LINE__);
		}
		
		formList.push_front(info);
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

		FormInfo info = { form, closeFunction };
		formList.push_front(info);
	}
	//---------------------------------------------------------------------------
	void FormManager::UnregisterForm(const std::shared_ptr<Form> &form)
	{
		if (form == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (formList.empty())
		{
			throw Misc::InvalidOperationException(L"FormList is empty.", __WFILE__, __LINE__);
		}

		for (auto it = formList.begin(); it != formList.end(); ++it)
		{
			FormInfo &info = *it;
			if (info.Form == form)
			{
				if (info.closeFunction != 0)
				{
					info.closeFunction();
				}
				formList.erase(it);
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes FormManager::ForwardEventToForms(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (formList.size() > 0)
		{
			std::shared_ptr<Form> foreMost = GetForeMost();
			if (foreMost != 0 && foreMost->IsModal())
			{
				return foreMost->ProcessEvent(event);
			}
			
			for (std::list<FormInfo>::itertor it = formList.begin(); it != formList.end(); ++it)
			{
				FormInfo &info = *it;
				if (info.form->ProcessEvent(event) == IEvent::DontContinue)
				{
					if (info.form != foreMost)
					{
						BringToFront(info.form);
					}

					return IEvent::DontContinue;
				}
			}
		}
		
		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void FormManager::RenderForms(Drawing::IRenderer *renderer)
	{
		if (removeForms.size() > 0)
		{
			for (std::list<Form*>::iterator it = removeForms.begin(); it != removeForms.end(); ++it)
			{
				delete *it;
			}
			removeForms.clear();
		}
		
		if (formList.size() > 0)
		{
			for (std::list<Form*>::iterator it = ++formList.begin(); it != formList.end(); ++it)
			{
				(*it)->Render(renderer);
			}

			GetForeMost()->Render(renderer);
		}
	}
	//---------------------------------------------------------------------------
}