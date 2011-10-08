#include "Application.h"
#include "Controls\Form.h"
#include "Controls\Timer.h"
#include "Drawing\TextureAnimator.h"
#include "Misc\Exceptions.h"
#include "FormManager.h"
#include "Cursor\Cursors.h"

namespace OSHGui
{
	std::map<Timer*, Application::TimerInfo> Application::timers;
	bool Application::enabled = false;
	Drawing::IRenderer *Application::renderer = 0;
	Application::MouseInfo Application::mouse;
	Misc::DateTime Application::now = Misc::DateTime::GetNow();
	FormManager Application::formManager;
	std::shared_ptr<Form> Application::mainForm;
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::IRenderer *renderer)
	{
		if (renderer == 0)
		{
			throw Misc::ArgumentNullException(L"renderer", __WFILE__, __LINE__);
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

		if (!mainForm->GetVisible())
		{
			mainForm->Show(mainForm);
		}
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		enabled = false;
	}
	//---------------------------------------------------------------------------
	void Application::Run(const std::shared_ptr<Form> &mainForm)
	{
		if (mainForm == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (Application::mainForm != 0)
		{
			return;
		}

		Application::mainForm = mainForm;

		mainForm->Show(mainForm);
	}
	//---------------------------------------------------------------------------
	void Application::RegisterTimer(Timer *timer, Misc::TimeSpan &interval)
	{
		if (timer == 0)
		{
			throw Misc::ArgumentNullException(L"timer", __WFILE__, __LINE__);
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
			throw Misc::ArgumentNullException(L"timer", __WFILE__, __LINE__);
		}
	
		std::map<Timer*, TimerInfo>::iterator it = timers.find(timer);
		if (it != timers.end())
		{
			timers.erase(it);
		}
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Application::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
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

		formManager.ForwardEventToForms(event);

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		if (renderer == 0)
		{
			throw Misc::InvalidOperationException(L"Call Application::Create first.", __WFILE__, __LINE__);
		}
	
		if (!enabled)
		{
			return;
		}
		
		now = Misc::DateTime::GetNow();

		Drawing::TextureAnimator::UpdateFrames();

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

		formManager.RenderForms(renderer);
		
		//render startbar
		renderer->SetRenderColor(Drawing::Color(0xFF5C5C5C));
		Drawing::Size size(584, 562);//renderer->GetDeviceSize();
		renderer->Fill(0, size.Height - 30, size.Width, 2);
		renderer->SetRenderColor(Drawing::Color(0xFF2D2D2D));
		renderer->FillGradient(0, size.Height - 28, size.Width, 28, Drawing::Color(0xFF121212));
		renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
		renderer->Fill(34, size.Height - 30, 1, 30);
		
		/*int tabWidth = (size.Width - 35) / forms.size();
		int tabPos = 35;
		for (std::list<Form*>::iterator it = forms.begin(); it != forms.end(); ++it, tabPos += tabWidth)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderText(renderer->GetDefaultFont(), tabPos + 4, size.Height - 21, tabWidth, 30, (*it)->GetText());
			renderer->SetRenderColor(Drawing::Color(0xFF2F2F2F));
			renderer->Fill(tabPos, size.Height - 30, 1, 30);
		}*/
		
		mouse.Cursor->Render(renderer, mouse.Position);
	}
	//---------------------------------------------------------------------------
}