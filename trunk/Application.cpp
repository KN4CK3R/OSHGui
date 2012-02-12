/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "Application.hpp"
#include "Controls/Form.hpp"
#include "Drawing/TextureAnimator.hpp"
#include "Misc/Exceptions.hpp"
#include "FormManager.hpp"
#include "TimerManager.hpp"
#include "Cursor/Cursors.hpp"

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
		CaptureControl = 0;
		MouseEnteredControl = 0;
	}
	//---------------------------------------------------------------------------
	Application* Application::Instance()
	{
		return instance;
	}
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::IRenderer *renderer)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (renderer == 0)
		{
			throw Misc::ArgumentNullException("renderer", __FILE__, __LINE__);
		}
		#endif
		
		this->renderer = renderer;

		mouse.Cursor = Cursors::Get(Cursors::Default);
	}
	//---------------------------------------------------------------------------
	const bool Application::IsEnabled() const
	{
		return isEnabled;
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
	const Drawing::Point& Application::GetCursorLocation() const
	{
		return mouse.Location;
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
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (mainForm == 0)
		{
			throw Misc::ArgumentNullException("form", __FILE__, __LINE__);
		}
		#endif

		if (formManager.GetMainForm() != 0)
		{
			return;
		}
		
		formManager.RegisterMainForm(mainForm);

		mainForm->Show(mainForm);
	}
	//---------------------------------------------------------------------------
	bool Application::ProcessMouseMessage(MouseMessage &mouse)
	{
		if (!isEnabled)
		{
			return false;
		}

		this->mouse.Location = mouse.Location;

		if (CaptureControl != 0)
		{
			CaptureControl->ProcessMouseMessage(mouse);
			return true;
		}
		if (FocusedControl != 0)
		{
			if (FocusedControl->ProcessMouseMessage(mouse))
			{
				return true;
			}
		}

		if (formManager.GetFormCount() > 0)
		{
			std::shared_ptr<Form> foreMost = formManager.GetForeMost();
			if (foreMost != 0 && foreMost->IsModal())
			{
				for (ContainerControl::PostOrderVisibleIterator it = foreMost->GetPostOrderVisibleEnumerator(); it(); ++it)
				{
					Control *control = *it;
					if (control->ProcessMouseMessage(mouse))
					{
						return true;
					}
				}
				return false;
			}
			
			for (FormManager::FormIterator it = formManager.GetEnumerator(); it(); ++it)
			{
				std::shared_ptr<Form> &form = *it;
				
				for (ContainerControl::PostOrderVisibleIterator it2 = form->GetPostOrderVisibleEnumerator(); it2(); ++it2)
				{
					Control *control = *it2;
					if (control->ProcessMouseMessage(mouse))
					{
						if (form != foreMost)
						{
							formManager.BringToFront(form);
						}

						return true;
					}
				}
			}
		}

		return false;
	}
	//---------------------------------------------------------------------------
	bool Application::ProcessKeyboardMessage(KeyboardMessage &keyboard)
	{
		if (!isEnabled)
		{
			return false;
		}

		if (FocusedControl != 0)
		{
			FocusedControl->ProcessKeyboardMessage(keyboard);
		}

		return false;
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (renderer == 0)
		{
			throw Misc::InvalidOperationException("Call Application::Create first.", __FILE__, __LINE__);
		}
		#endif
	
		if (!isEnabled)
		{
			return;
		}
		
		now = Misc::DateTime::GetNow();

		Drawing::TextureAnimator::UpdateFrames();

		timerManager.Update();

		formManager.RemoveUnregisteredForms();

		std::shared_ptr<Form> foreMost = formManager.GetForeMost();
		for (FormManager::FormIterator it = formManager.GetEnumerator(); it(); ++it)
		{
			std::shared_ptr<Form> &form = *it;
			if (form != foreMost)
			{
				form->Render(renderer);
			}
		}
		if (foreMost)
		{
			foreMost->Render(renderer);
		}
		
		mouse.Cursor->Render(renderer, mouse.Location);
	}
	//---------------------------------------------------------------------------
}