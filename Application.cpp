#include "Application.hpp"
#include "Controls\Form.hpp"
#include "Drawing\TextureAnimator.hpp"
#include "Misc\Exceptions.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"
#include "Cursor\Cursors.hpp"

namespace OSHGui
{
	bool Application::enabled = false;
	Drawing::IRenderer *Application::renderer = 0;
	Application::MouseInfo Application::mouse;
	Misc::DateTime Application::now = Misc::DateTime::GetNow();
	FormManager Application::formManager;
	TimerManager Application::timerManager;
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
		enabled = false;
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

		timerManager.Update();

		formManager.RenderForms(renderer);
		
		mouse.Cursor->Render(renderer, mouse.Position);
	}
	//---------------------------------------------------------------------------
}