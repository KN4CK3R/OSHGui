/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "Application.hpp"
#include "Controls/Form.hpp"
#include "Drawing/TextureAnimator.hpp"
#include "Misc/Exceptions.hpp"
#include "FormManager.hpp"
#include "Cursor/Cursors.hpp"
#include <algorithm>

#include "FreeImage.h"

namespace OSHGui
{
	Application* Application::instance = nullptr;
	//---------------------------------------------------------------------------
	Application::Application(Drawing::Renderer &_renderer)
		: renderer(_renderer),
		  guiSurface(*renderer.GetDefaultRenderTarget()),
		  isEnabled(false),
		  now(Misc::DateTime::GetNow()),
		  FocusedControl(nullptr),
		  CaptureControl(nullptr),
		  MouseEnteredControl(nullptr)
	{
		FreeImage_Initialise();

		#define MakeTheme(controlType, color1, color2) defaultTheme.SetControlColorTheme(Control::ControlTypeToString(controlType), Drawing::Theme::ControlTheme(color1, color2))

		MakeTheme(ControlType::Label,		Drawing::Color::White(), Drawing::Color::Empty());
		MakeTheme(ControlType::LinkLabel,	Drawing::Color::White(), Drawing::Color::Empty());
		MakeTheme(ControlType::Button,		Drawing::Color::White(), Drawing::Color(0xFF4E4E4E));
		MakeTheme(ControlType::CheckBox,	Drawing::Color::White(), Drawing::Color(0xFF222222));
		MakeTheme(ControlType::RadioButton, Drawing::Color::White(), Drawing::Color(0xFF222222));
		MakeTheme(ControlType::ColorBar,	Drawing::Color::White(), Drawing::Color::Empty());
		MakeTheme(ControlType::ColorPicker,	Drawing::Color::Empty(),	Drawing::Color::Empty());
		MakeTheme(ControlType::ComboBox,	Drawing::Color::White(), Drawing::Color(0xFF4E4E4E));
		MakeTheme(ControlType::Form,		Drawing::Color::White(), Drawing::Color(0xFF7C7B79));
		MakeTheme(ControlType::GroupBox,	Drawing::Color::White(), Drawing::Color::Empty());
		MakeTheme(ControlType::ListBox,		Drawing::Color::White(), Drawing::Color(0xFF171614));
		MakeTheme(ControlType::Panel,		Drawing::Color::Empty(),	Drawing::Color::Empty());
		MakeTheme(ControlType::PictureBox,	Drawing::Color::Empty(),	Drawing::Color::Empty());
		MakeTheme(ControlType::ProgressBar,	Drawing::Color(0xFF5A5857),	Drawing::Color::Empty());
		MakeTheme(ControlType::ScrollBar,	Drawing::Color(0xFFAFADAD), Drawing::Color(0xFF585552));
		MakeTheme(ControlType::TabControl,	Drawing::Color::White(), Drawing::Color(0xFF737373));
		MakeTheme(ControlType::TabPage,		Drawing::Color::White(), Drawing::Color(0xFF474747));
		MakeTheme(ControlType::TextBox,		Drawing::Color::White(), Drawing::Color(0xFF242321));
		MakeTheme(ControlType::TrackBar,	Drawing::Color::White(), Drawing::Color::Empty());
		MakeTheme(ControlType::HotkeyControl, Drawing::Color::White(), Drawing::Color(0xFF242321));

		SetTheme(defaultTheme);
	}
	//---------------------------------------------------------------------------
	Application& Application::Instance()
	{
		return *instance;
	}
	//---------------------------------------------------------------------------
	void Application::Create(Drawing::Renderer &renderer)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (instance)
		{
			throw Misc::InvalidOperationException("only one instance");
		}
		#endif

		instance = new Application(renderer);

		instance->mouse.Enabled = true;
		instance->SetCursor(Cursors::Get(Cursors::Default));
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
	Drawing::Renderer& Application::GetRenderer() const
	{
		return renderer;
	}
	//---------------------------------------------------------------------------
	Application::GuiRenderSurface& Application::GetRenderSurface()
	{
		return guiSurface;
	}
	//---------------------------------------------------------------------------
	Drawing::FontPtr& Application::GetDefaultFont()
	{
		return defaultFont;
	}
	//---------------------------------------------------------------------------
	void Application::SetDefaultFont(Drawing::FontPtr &_defaultFont)
	{
		defaultFont = _defaultFont;

		guiSurface.Invalidate();
	}
	//---------------------------------------------------------------------------
	const Drawing::PointF& Application::GetCursorLocation() const
	{
		return mouse.Location;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor>& Application::GetCursor() const
	{
		return mouse.Cursor;
	}
	//---------------------------------------------------------------------------
	void Application::SetCursor(const CursorPtr &cursor)
	{
		mouse.Cursor = cursor ? cursor : Cursors::Get(Cursors::Default);

		guiSurface.Invalidate();
	}
	//---------------------------------------------------------------------------
	void Application::SetCursorEnabled(bool enabled)
	{
		mouse.Enabled = enabled;

		guiSurface.Invalidate();
	}
	//---------------------------------------------------------------------------
	void Application::SetTheme(const Drawing::Theme &theme)
	{
		currentTheme = theme;
		for (auto it = formManager.GetEnumerator(); it(); ++it)
		{
			auto &form = *it;
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

		auto mainForm = formManager.GetMainForm();
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
			throw Misc::ArgumentNullException("form");
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
	bool Application::ProcessMouseMessage(MouseMessage &message)
	{
		if (!isEnabled)
		{
			return false;
		}

		mouse.Location = message.Location;

		if (CaptureControl != nullptr)
		{
			CaptureControl->ProcessMouseMessage(message);
			return true;
		}
		if (FocusedControl != nullptr)
		{
			if (FocusedControl->ProcessMouseMessage(message))
			{
				return true;
			}
		}

		if (formManager.GetFormCount() > 0)
		{
			auto foreMost = formManager.GetForeMost();
			if (foreMost != nullptr && foreMost->IsModal())
			{
				for (auto it = foreMost->GetPostOrderEnumerator(); it(); ++it)
				{
					auto control = *it;
					if (control->ProcessMouseMessage(message))
					{
						return true;
					}
				}
				return false;
			}
			
			for (auto it = formManager.GetEnumerator(); it(); ++it)
			{
				auto &form = *it;
				
				for (auto it2 = form->GetPostOrderEnumerator(); it2(); ++it2)
				{
					auto control = *it2;
					if (control->ProcessMouseMessage(message))
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
				MouseEnteredControl->OnMouseLeave(message);
			}
		}

		return false;
	}
	//---------------------------------------------------------------------------
	bool Application::ProcessKeyboardMessage(KeyboardMessage &keyboard)
	{
		if (keyboard.GetState() == KeyboardMessage::KeyDown)
		{
			bool hotkeyFired = false;
			for (auto it = hotkeys.begin(); it != hotkeys.end(); ++it)
			{
				Hotkey &temp = *it;
				if (temp.GetKey() == keyboard.GetKeyCode() && temp.GetModifier() == keyboard.GetModifier())
				{
					hotkeyFired = true;
					temp();
				}
			}

			if (hotkeyFired)
			{
				return true;
			}
		}

		if (isEnabled)
		{
			if (FocusedControl != nullptr)
			{
				return FocusedControl->ProcessKeyboardMessage(keyboard);
			}
		}

		return false;
	}
	//---------------------------------------------------------------------------
	void Application::InjectTime()
	{
		now = Misc::DateTime::GetNow();

		for (auto it = formManager.GetEnumerator(); it(); ++it)
		{
			(*it)->InjectTime(now);

			for (auto it2 = (*it)->GetPostOrderEnumerator(); it2(); ++it2)
			{
				(*it2)->InjectTime(now);
			}
		}
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		if (!isEnabled)
		{
			return;
		}
		
		InjectTime();

		//Drawing::TextureAnimator::UpdateFrames();

		formManager.RemoveUnregisteredForms();

		if (guiSurface.needsRedraw)
		{
			auto foreMost = formManager.GetForeMost();
			for (auto it = formManager.GetEnumerator(); it(); ++it)
			{
				auto &form = *it;
				if (form != foreMost)
				{
					form->Render();
				}
			}
			if (foreMost)
			{
				foreMost->Render();
			}

			guiSurface.AddGeometry(Drawing::RenderQueueType::Overlay, mouse.Cursor->GetGeometry());

			guiSurface.needsRedraw = false;
		}

		if (mouse.Enabled)
		{
			mouse.Cursor->GetGeometry()->SetTranslation(Drawing::Vector(mouse.Location.X, mouse.Location.Y, 0));
		}

		guiSurface.Draw();
	}
	//---------------------------------------------------------------------------
	void Application::RegisterHotkey(const Hotkey &hotkey)
	{
		UnregisterHotkey(hotkey);

		hotkeys.push_back(hotkey);
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterHotkey(const Hotkey &hotkey)
	{
		hotkeys.erase(std::remove(std::begin(hotkeys), std::end(hotkeys), hotkey), std::end(hotkeys));
	}
	//---------------------------------------------------------------------------
	//Application::GuiRenderSurface
	//---------------------------------------------------------------------------
	Application::GuiRenderSurface::GuiRenderSurface(Drawing::RenderTarget &target)
		: RenderSurface(target),
		  needsRedraw(true)
	{

	}
	//---------------------------------------------------------------------------
	void Application::GuiRenderSurface::Invalidate()
	{
		needsRedraw = true;

		Reset();
	}
	//---------------------------------------------------------------------------
	void Application::GuiRenderSurface::Draw()
	{
		if (needsRedraw)
		{

		}

		RenderSurface::Draw();
	}
	//---------------------------------------------------------------------------

}
