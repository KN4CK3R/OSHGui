#include "Application.h"
#include "Form.h"
#include "Timer.h"
#include "Drawing\TextureAnimator.h"
#include "Misc\Exceptions.h"

namespace OSHGui
{
	std::map<Timer*, Application::TimerInfo> Application::timers;
	std::list<Form*> Application::forms;
	std::list<Application::ModalInfo> Application::modals;
	std::list<Form*> Application::removeForms;
	Form *Application::focusForm = 0;
	Form *Application::mainForm = 0;
	bool Application::enabled = false;
	Drawing::IRenderer *Application::renderer = 0;
	Application::MouseInfo Application::mouse;
	Misc::DateTime Application::now = Misc::DateTime::GetNow();
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::IRenderer *renderer)
	{
		if (renderer == 0)
		{
			throw ArgumentNullException(L"renderer", __WFILE__, __LINE__);
		}
		
		Application::renderer = renderer;

		mouse.Cursor = Cursors::Get(Cursors::Default);
	}
	//---------------------------------------------------------------------------
	const Misc::DateTime& Application::GetNow()
	{
		return now;
	}
	//---------------------------------------------------------------------------
	Drawing::IRenderer* Application::GetRenderer()
	{
		return renderer;
	}
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
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
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
		if (timer == 0)
		{
			throw ArgumentNullException(L"timer", __WFILE__, __LINE__);
		}
	
		if (timers.find(timer) == timers.end())
		{
			TimerInfo info;
			info.timer = timer;
			info.interval = interval;
			info.next = now.Add(interval);
			timers[timer] = info;
		}
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterTimer(Timer *timer)
	{
		if (timer == 0)
		{
			throw ArgumentNullException(L"timer", __WFILE__, __LINE__);
		}
	
		std::map<Timer*, TimerInfo>::iterator it = timers.find(timer);
		if (it != timers.end())
		{
			timers.erase(it);
		}
	}
	//---------------------------------------------------------------------------
	void Application::RegisterForm(Form *form, const std::function<void()> &modalFunc)
	{
		if (form == 0)
		{
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		for (std::list<Form*>::iterator it = removeForms.begin(); it != removeForms.end(); ++it)
		{
			if (*it == form)
			{
				removeForms.erase(it);
				break;
			}
		}

		for (std::list<ModalInfo>::iterator it = modals.begin(); it != modals.end(); ++it)
		{
			if (it->form == form)
			{
				return;
			}
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it)
		{
			if (*it == form)
			{
				return;
			}
		}

		if (form->IsModal())
		{
			ModalInfo info;
			info.form = form;
			info.func = modalFunc;

			modals.push_front(info);
		}
		else
		{
			forms.push_front(form);
			focusForm = form;
		}
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterForm(Form *form)
	{
		if (form == 0)
		{
			throw ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (form == mainForm) //don't unregister mainForm
		{
			return;
		}

		if (modals.size() > 0)
		{
			if (modals.front().form == form)
			{
				ModalInfo info = modals.front();
				modals.pop_front();
				
				removeForms.push_back(info.form);

				if (info.func)
				{
					info.func();
				}
			}
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it)
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
	void Application::BringToFront(Form *form)
	{
		if (form != 0)
		{
			focusForm = form;
		}
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Application::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (!enabled)
		{
			return IEvent::Continue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			//grab mouse position here for cursor rendering
			Application::mouse.Position = mouse->Position;
		}

		if (modals.size() != 0)
		{
			modals.front().form->ProcessEvent(event);
			return IEvent::DontContinue;
		}
		
		if (focusForm != 0 && focusForm->ProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}
		else
		{
			for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it)
			{
				if (*it != focusForm)
				{
					if ((*it)->ProcessEvent(event) == IEvent::DontContinue)
					{
						focusForm = *it;

						return IEvent::DontContinue;
					}
				}
			}
		}

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		if (renderer == 0)
		{
			throw InvalidOperationException(L"Call Application::Create first.", __WFILE__, __LINE__);
		}
	
		if (!enabled)
		{
			return;
		}
		
		now = Misc::DateTime::GetNow();

		Drawing::TextureAnimator::UpdateFrames();

		if (removeForms.size() > 0)
		{
			for (std::list<Form*>::iterator it = removeForms.begin(); it != removeForms.end(); ++it)
			{
				delete *it;
			}
			removeForms.clear();
		}

		if (timers.size() > 0)
		{
			for (std::map<Timer*, TimerInfo>::iterator it = timers.begin(); it != timers.end(); ++it)
			{
				TimerInfo &info = it->second;
				if (info.next < now)
				{
					it->first->tickEvent.Invoke(it->first);
					info.next = now.Add(info.interval);
				}
			}
		}

		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it)
		{
			if (*it != focusForm)
			{
				(*it)->Render(renderer);
			}
		}

		if (focusForm != 0)
		{
			focusForm->Render(renderer);
		}

		if (modals.size() > 0)
		{
			modals.front().form->Render(renderer);
		}
		
		//render startbar
		renderer->SetRenderColor(Drawing::Color(0xFF5C5C5C));
		Drawing::Size size(584, 562);//renderer->GetDeviceSize();
		renderer->Fill(0, size.Height - 30, size.Width, 2);
		renderer->SetRenderColor(Drawing::Color(0xFF2D2D2D));
		renderer->FillGradient(0, size.Height - 28, size.Width, 28, Drawing::Color(0xFF121212));
		renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
		renderer->Fill(34, size.Height - 30, 1, 30);
		
		int tabWidth = (size.Width - 35) / forms.size();
		int tabPos = 35;
		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it, tabPos += tabWidth)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderText(renderer->GetDefaultFont(), tabPos + 4, size.Height - 21, tabWidth, 30, (*it)->GetText());
			renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
			renderer->Fill(tabPos, size.Height - 30, 1, 30);
		}
		
		mouse.Cursor->Render(renderer, mouse.Position);
	}
	//---------------------------------------------------------------------------
}