#include "Application.h"
#include "Form.h"

namespace OSHGui
{
	std::list<Form*> Application::forms;
	Form *Application::focusForm = NULL;
	bool Application::enabled = false;
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		enabled = true;
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		enabled = false;
	}
	//---------------------------------------------------------------------------
	void Application::RegisterForm(Form *form)
	{
		if (form == NULL)
		{
			return;
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
		{
			if (*it == form)
			{
				return;
			}
		}

		forms.push_front(form);

		focusForm = form;
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterForm(Form *form)
	{
		if (form == NULL)
		{
			return;
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
		{
			if (*it == form)
			{
				it = forms.erase(it);

				if (focusForm == form)
				{
					if (forms.size() > 0)
					{
						focusForm = *(--it);
					}
					else
					{
						focusForm = NULL;
					}
				}

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	Event::NextEventTypes Application::ProcessEvent(Event *event)
	{
		if (event == NULL || !enabled)
		{
			return Event::Continue;
		}
		
		if (focusForm != NULL && focusForm->ProcessEvent(event) == Event::DontContinue)
		{
			return Event::DontContinue;
		}
		else
		{
			for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
			{
				if (*it != focusForm)
				{
					if ((*it)->ProcessEvent(event) == Event::DontContinue)
					{
						focusForm = *it;

						return Event::DontContinue;
					}
				}
			}
		}

		return Event::Continue;
	}
	//---------------------------------------------------------------------------
	void Application::Render(Drawing::IRenderer *renderer)
	{
		if (!enabled || renderer == NULL)
		{
			return;
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
		{
			if (*it != focusForm)
			{
				(*it)->Render(renderer);
			}
		}

		focusForm->Render(renderer);
	}
	//---------------------------------------------------------------------------
}