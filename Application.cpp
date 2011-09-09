#include "Application.h"
#include "Form.h"
#include "Timer.h"

namespace OSHGui
{
	std::map<Timer*, Application::TimerInfo> Application::timers;
	std::list<Form*> Application::forms;
	Form *Application::focusForm = 0;
	Form *Application::mainForm = 0;
	std::list<Form*> Application::removeForms;
	bool Application::enabled = false;
	Drawing::IRenderer* Application::Renderer = 0;
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		enabled = true;

		if (mainForm != 0)
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
		if (form == 0)
		{
			return;
		}

		if (mainForm != 0)
		{
			return;
		}

		RegisterForm(form);

		mainForm = form;
	}
	//---------------------------------------------------------------------------
	void Application::RegisterTimer(Timer *timer, Misc::TimeSpan &interval)
	{
		if (timers.find(timer) == timers.end())
		{
			TimerInfo info;
			info.timer = timer;
			info.interval = interval;
			info.next = Misc::DateTime::GetNow().Add(interval);
			timers[timer] = info;
		}
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterTimer(Timer *timer)
	{
		std::map<Timer*, TimerInfo>::iterator it = timers.find(timer);
		if (it != timers.end())
		{
			timers.erase(it);
		}
	}
	//---------------------------------------------------------------------------
	void Application::RegisterForm(Form *form)
	{
		if (form == 0)
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
		if (form == 0)
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

				if (focusForm == form)
				{
					if (forms.size() > 0)
					{
						focusForm = *it;
					}
					else
					{
						focusForm = 0;
					}
				}

				removeForms.push_back(form);

				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	Event::NextEventTypes Application::ProcessEvent(Event *event)
	{
		if (event == 0 || !enabled)
		{
			return Event::Continue;
		}
		
		if (focusForm != 0 && focusForm->ProcessEvent(event) == Event::DontContinue)
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
	void Application::Render()
	{
		if (!enabled || Renderer == 0)
		{
			return;
		}

		if (removeForms.size() > 0)
		{
			for (std::list<Form*>::iterator it = removeForms.begin(); it != removeForms.end(); it++)
			{
				delete *it;
			}
			removeForms.clear();
		}

		if (timers.size() > 0)
		{
			Misc::DateTime now = Misc::DateTime::GetNow();
			for (std::map<Timer*, TimerInfo>::iterator it = timers.begin(); it != timers.end(); it++)
			{
				TimerInfo &info = it->second;
				if (info.next < now)
				{
					it->first->tickEventHandler.Invoke(0);
					info.next = now.Add(info.interval);
				}
			}
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); it++)
		{
			if (*it != focusForm)
			{
				(*it)->Render(Renderer);
			}
		}

		focusForm->Render(Renderer);
	}
	//---------------------------------------------------------------------------
}