#include "Application.hpp"
#include "Controls\Form.hpp"
#include "Drawing\TextureAnimator.hpp"
#include "Misc\Exceptions.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"
#include "Cursor\Cursors.hpp"

namespace OSHGui
{
	Application* Application::instance = new Application();
	//---------------------------------------------------------------------------
	Application::Application()
	{
		isEnabled = false;
		renderer = 0;
		now = Misc::DateTime::GetNow();
		FocusedControl = 0;
		ClickedControl = 0;
		CaptureControl = 0;
		MouseEnteredControl = 0;
	}
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::IRenderer *renderer)
	{
		if (renderer == 0)
		{
			throw Misc::ArgumentNullException(L"renderer", __WFILE__, __LINE__);
		}
		
		this->renderer = renderer;

		mouse.Cursor = Cursors::Get(Cursors::Default);
	}
	//---------------------------------------------------------------------------
	const Misc::DateTime& Application::GetNow() const
	{
		return now;
	}
	//---------------------------------------------------------------------------
	Drawing::IRenderer* Application::GetRenderer() const
	{
		return renderer;
	}
	//---------------------------------------------------------------------------
	const Drawing::Point& Application::GetCursorPosition() const
	{
		return mouse.Position;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor>& Application::GetCursor() const
	{
		return mouse.Cursor;
	}
	//---------------------------------------------------------------------------
	void Application::SetCursor(const std::shared_ptr<Cursor> &cursor)
	{
		mouse.Cursor = cursor;
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
	bool Application::ProcessMouseEvent(MouseMessage &mouse)
	{
		if (!isEnabled)
		{
			return false;
		}

		if (mouse.State != MouseMessage::Scroll)
		{
			mouse.Position = mouse.Position;
		}

		if (CaptureControl != 0)
		{
			CaptureControl->ProcessMouseEvent(mouse);
			return true;
		}

		if (formManager.GetFormCount() > 0)
		{
			std::shared_ptr<Form> foreMost = formManager.GetForeMost();
			if (foreMost != 0 && foreMost->IsModal())
			{
				return foreMost->ProcessMouseEvent(mouse);
			}
			
			for (FormManager::FormIterator it = formManager.GetEnumerator(); it(); ++it)
			{
				std::shared_ptr<Form> &form = *it;
				
				if (form->ProcessMouseEvent(mouse) == true)
				{
					if (form != foreMost)
					{
						formManager.BringToFront(form);
					}

					return true;
				}
			}
		}

		return false;
	}
	bool Application::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}
		
		if (!isEnabled)
		{
			return false;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseMessage *mouse = (MouseMessage*)event;
			//grab mouse position here for cursor rendering
			if (mouse->State != MouseMessage::Scroll)
			{
				mouse.Position = mouse->Position;
			}
		}

		formManager.ForwardEventToForms(event);
		
		return false;
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