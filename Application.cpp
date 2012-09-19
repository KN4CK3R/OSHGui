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
		: isEnabled(false),
		  renderer(nullptr),
		  now(Misc::DateTime::GetNow()),
		  FocusedControl(nullptr),
		  CaptureControl(nullptr),
		  MouseEnteredControl(nullptr)
	{
		#define MakeTheme(control, color1, color2) defaultTheme.SetControlColorTheme(control, Drawing::Theme::ControlTheme(color1, color2))

		MakeTheme(CONTROL_LABEL, 0xFFE5E0E4, Drawing::Color::Empty());
		MakeTheme(CONTROL_LINKLABEL, 0xFFE5E0E4, Drawing::Color::Empty());
		MakeTheme(CONTROL_BUTTON, 0xFFE5E0E4, 0xFF4E4E4E);
		MakeTheme(CONTROL_CHECKBOX, 0xFFE5E0E4, 0xFF222222);
		MakeTheme(CONTROL_RADIOBUTTON, 0xFFE5E0E4, 0xFF222222);
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
		if (renderer == nullptr)
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
	void Application::SetCursorEnabled(bool enabled)
	{
		mouse.Enabled = enabled;
	}
	//---------------------------------------------------------------------------
	void Application::SetTheme(const Drawing::Theme &theme)
	{
		currentTheme = theme;
		for (FormManager::FormIterator it = formManager.GetEnumerator(); it(); ++it)
		{
			std::shared_ptr<Form> &form = *it;
			form->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Theme& Application::GetTheme() const
	{
		return currentTheme;
	}
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		isEnabled = true;

		std::shared_ptr<Form> mainForm = formManager.GetMainForm();
		if (mainForm != nullptr)
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
		if (mainForm == nullptr)
		{
			throw Misc::ArgumentNullException("form", __FILE__, __LINE__);
		}
		#endif

		if (formManager.GetMainForm() != nullptr)
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

		if (CaptureControl != nullptr)
		{
			CaptureControl->ProcessMouseMessage(mouse);
			return true;
		}
		if (FocusedControl != nullptr)
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

			if (MouseEnteredControl)
			{
				MouseEnteredControl->OnMouseLeave(mouse);
			}
		}

		return false;
	}
	//---------------------------------------------------------------------------
	bool Application::ProcessKeyboardMessage(KeyboardMessage &keyboard)
	{
		bool processed = false;
		if (isEnabled)
		{
			if (FocusedControl != nullptr)
			{
				processed = FocusedControl->ProcessKeyboardMessage(keyboard);
			}
		}

		if (keyboard.State == KeyboardMessage::KeyUp)
		{
			for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it)
			{
				Hotkey &temp = *it;
				if (temp.GetKey() == keyboard.KeyCode && temp.GetModifier() == keyboard.Modifier)
				{
					temp();
				}
			}
		}

		return processed;
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (renderer == nullptr)
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
		
		if (mouse.Enabled)
		{
			mouse.Cursor->Render(renderer, mouse.Location);
		}
	}
	//---------------------------------------------------------------------------
	void Application::RegisterHotkey(const Hotkey &hotkey)
	{
		for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it)
		{
			Hotkey &temp = *it;
			if (temp.GetKey() == hotkey.GetKey() && temp.GetModifier() == hotkey.GetModifier())
			{
				it = hotkeys.erase(it);
				hotkeys.insert(it, hotkey);
				return;
			}
		}
		hotkeys.push_back(hotkey);
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterHotkey(const Hotkey &hotkey)
	{
		for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it)
		{
			Hotkey &temp = *it;
			if (temp.GetKey() == hotkey.GetKey() && temp.GetModifier() == hotkey.GetModifier())
			{
				hotkeys.erase(it);
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
}