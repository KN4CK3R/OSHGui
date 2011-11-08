#include "Application.hpp"
#include "Controls\Form.hpp"
#include "Drawing\TextureAnimator.hpp"
#include "Misc\Exceptions.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"
#include "Cursor\Cursors.hpp"

namespace OSHGui
{
	bool Application::isEnabled = false;
	Drawing::IRenderer *Application::renderer = 0;
	Application::MouseInfo Application::mouse;
	Misc::DateTime Application::now = Misc::DateTime::GetNow();
	FormManager Application::formManager;
	TimerManager Application::timerManager;
	Control *Application::FocusedControl = 0;
	Control *Application::CapturedControl = 0;
	Control *Application::MouseEnteredControl = 0;
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
		isEnabled = true;

		std::shared_ptr<Form> mainForm = formManager.GetMainForm();
		if (mainForm != 0)
		{
			if (!formManager.IsRegistered(mainForm))
			{
				mainForm->Show(mainForm);
			}
		}
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		isEnabled = false;
	}
	//---------------------------------------------------------------------------
	void Application::Toggle()
	{
		if (isEnabled)
		{
			Disable();
		}
		else
		{
			Enable();
		}
	}
	//---------------------------------------------------------------------------
	void Application::Run(const std::shared_ptr<Form> &mainForm)
	{
		if (mainForm == 0)
		{
			throw Misc::ArgumentNullException(L"form", __WFILE__, __LINE__);
		}

		if (formManager.GetMainForm() != 0)
		{
			return;
		}
		
		formManager.RegisterMainForm(mainForm);

		mainForm->Show(mainForm);
	}
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes Application::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (!isEnabled)
		{
			return IEvent::Continue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			//grab mouse position here for cursor rendering
			if (mouse->State != MouseEvent::Scroll)
			{
				Application::mouse.Position = mouse->Position;
			}
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
	
		if (!isEnabled)
		{
			return;
		}
		
		now = Misc::DateTime::GetNow();

		Drawing::TextureAnimator::UpdateFrames();

		timerManager.Update();

		formManager.RenderForms(renderer);
		
		mouse.Cursor->Render(renderer, mouse.Position);
	}
	//---------------------------------------------------------------------------
}