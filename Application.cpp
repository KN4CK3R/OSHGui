#include "Application.h"
#include "Form.h"

namespace OSHGui
{
	std::list<Form*> Application::forms;
	Form *Application::focusForm = NULL;
	Form *Application::mainForm = NULL;
	bool Application::enabled = false;
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		enabled = true;

		if (mainForm != NULL)
		{
			mainForm->Show();
		}
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		enabled = false;
	}
	//---------------------------------------------------------------------------
	void Application::Run(Form *form)
	{
		if (form == NULL)
		{
			return;
		}

		if (mainForm != NULL)
		{
			return;
		}

		RegisterForm(form);

		mainForm = form;
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

		if (form == mainForm) //don't unregister mainForm
		{
			return;
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
		{
			if (*it == form)
			{
				it = forms.erase(it);

				delete form;

				if (focusForm == form)
				{
					if (forms.size() > 0)
					{
						focusForm = *it;
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