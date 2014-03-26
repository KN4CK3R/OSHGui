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
	Application::Application(Drawing::Renderer &renderer)
		: renderer_(renderer),
		  guiSurface_(*renderer.GetDefaultRenderTarget()),
		  isEnabled_(false),
		  now_(Misc::DateTime::GetNow()),
		  FocusedControl(nullptr),
		  CaptureControl(nullptr),
		  MouseEnteredControl(nullptr)
	{
		FreeImage_Initialise();

		#define MakeTheme(controlType, color1, color2) defaultTheme_.SetControlColorTheme(Control::ControlTypeToString(controlType), Drawing::Theme::ControlTheme(color1, color2))

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

		SetTheme(defaultTheme_);
	}
	//---------------------------------------------------------------------------
	Application& Application::Instance()
	{
		return *instance;
	}
	//---------------------------------------------------------------------------
	Application* Application::InstancePtr()
	{
		return instance;
	}
	//---------------------------------------------------------------------------
	bool Application::HasBeenInitialized()
	{
		return InstancePtr() != nullptr;
	}
	//---------------------------------------------------------------------------
	void Application::Initialize(Drawing::Renderer &renderer)
	{
		#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (HasBeenInitialized())
		{
			throw Misc::InvalidOperationException("only one instance");
		}
		#endif

		instance = new Application(renderer);

		instance->mouse_.Enabled = true;
		instance->SetCursor(Cursors::Get(Cursors::Default));
	}
	//---------------------------------------------------------------------------
	const bool Application::IsEnabled() const
	{
		return isEnabled_;
	}
	//---------------------------------------------------------------------------
	const Misc::DateTime& Application::GetNow() const
	{
		return now_;
	}
	//---------------------------------------------------------------------------
	Drawing::Renderer& Application::GetRenderer() const
	{
		return renderer_;
	}
	//---------------------------------------------------------------------------
	Application::GuiRenderSurface& Application::GetRenderSurface()
	{
		return guiSurface_;
	}
	//---------------------------------------------------------------------------
	Drawing::FontPtr& Application::GetDefaultFont()
	{
		return defaultFont_;
	}
	//---------------------------------------------------------------------------
	void Application::SetDefaultFont(const Drawing::FontPtr &defaultFont)
	{
		defaultFont_ = defaultFont;

		guiSurface_.Invalidate();
	}
	//---------------------------------------------------------------------------
	const Drawing::PointF& Application::GetCursorLocation() const
	{
		return mouse_.Location;
	}
	//---------------------------------------------------------------------------
	const std::shared_ptr<Cursor>& Application::GetCursor() const
	{
		return mouse_.Cursor;
	}
	//---------------------------------------------------------------------------
	void Application::SetCursor(const CursorPtr &cursor)
	{
		mouse_.Cursor = cursor ? cursor : Cursors::Get(Cursors::Default);

		guiSurface_.Invalidate();
	}
	//---------------------------------------------------------------------------
	void Application::SetCursorEnabled(bool enabled)
	{
		mouse_.Enabled = enabled;

		guiSurface_.Invalidate();
	}
	//---------------------------------------------------------------------------
	void Application::SetTheme(const Drawing::Theme &theme)
	{
		currentTheme_ = theme;
		for (auto it = formManager_.GetEnumerator(); it(); ++it)
		{
			auto &form = *it;
			form->ApplyTheme(theme);
		}
	}
	//---------------------------------------------------------------------------
	const Drawing::Theme& Application::GetTheme() const
	{
		return currentTheme_;
	}
	//---------------------------------------------------------------------------
	void Application::Enable()
	{
		isEnabled_ = true;

		auto mainForm = formManager_.GetMainForm();
		if (mainForm != nullptr)
		{
			if (!formManager_.IsRegistered(mainForm))
			{
				mainForm->Show(mainForm);
			}
		}
	}
	//---------------------------------------------------------------------------
	void Application::Disable()
	{
		isEnabled_ = false;
	}
	//---------------------------------------------------------------------------
	void Application::Toggle()
	{
		if (isEnabled_)
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

		if (formManager_.GetMainForm() != nullptr)
		{
			return;
		}
		
		formManager_.RegisterMainForm(mainForm);

		mainForm->Show(mainForm);
	}
	//---------------------------------------------------------------------------
	bool Application::ProcessMouseMessage(MouseMessage &message)
	{
		if (!isEnabled_)
		{
			return false;
		}

		mouse_.Location = message.GetLocation();

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

		if (formManager_.GetFormCount() > 0)
		{
			auto foreMost = formManager_.GetForeMost();
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
			
			for (auto it = formManager_.GetEnumerator(); it(); ++it)
			{
				auto &form = *it;
				
				for (auto it2 = form->GetPostOrderEnumerator(); it2(); ++it2)
				{
					auto control = *it2;
					if (control->ProcessMouseMessage(message))
					{
						if (form != foreMost)
						{
							formManager_.BringToFront(form);
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
		if (keyboard.GetState() == KeyboardState::KeyDown)
		{
			bool hotkeyFired = false;
			for (auto &hotkey : hotkeys_)
			{
				if (hotkey.GetKey() == keyboard.GetKeyCode() && hotkey.GetModifier() == keyboard.GetModifier())
				{
					hotkeyFired = true;
					hotkey();
				}
			}

			if (hotkeyFired)
			{
				return true;
			}
		}

		if (isEnabled_)
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
		now_ = Misc::DateTime::GetNow();

		for (auto it = formManager_.GetEnumerator(); it(); ++it)
		{
			(*it)->InjectTime(now_);

			for (auto it2 = (*it)->GetPostOrderEnumerator(); it2(); ++it2)
			{
				(*it2)->InjectTime(now_);
			}
		}
	}
	//---------------------------------------------------------------------------
	void Application::Render()
	{
		if (!isEnabled_)
		{
			return;
		}
		
		InjectTime();

		//Drawing::TextureAnimator::UpdateFrames();

		formManager_.RemoveUnregisteredForms();

		if (guiSurface_.needsRedraw_)
		{
			auto foreMost = formManager_.GetForeMost();
			for (auto it = formManager_.GetEnumerator(); it(); ++it)
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

			guiSurface_.AddGeometry(Drawing::RenderQueueType::Overlay, mouse_.Cursor->GetGeometry());

			guiSurface_.needsRedraw_ = false;
		}

		if (mouse_.Enabled)
		{
			mouse_.Cursor->GetGeometry()->SetTranslation(Drawing::Vector(mouse_.Location.X, mouse_.Location.Y, 0));
		}

		guiSurface_.Draw();
	}
	//---------------------------------------------------------------------------
	void Application::RegisterHotkey(const Hotkey &hotkey)
	{
		UnregisterHotkey(hotkey);

		hotkeys_.push_back(hotkey);
	}
	//---------------------------------------------------------------------------
	void Application::UnregisterHotkey(const Hotkey &hotkey)
	{
		hotkeys_.erase(std::remove(std::begin(hotkeys_), std::end(hotkeys_), hotkey), std::end(hotkeys_));
	}
	//---------------------------------------------------------------------------
	//Application::GuiRenderSurface
	//---------------------------------------------------------------------------
	Application::GuiRenderSurface::GuiRenderSurface(Drawing::RenderTarget &target)
		: RenderSurface(target),
		  needsRedraw_(true)
	{

	}
	//---------------------------------------------------------------------------
	void Application::GuiRenderSurface::Invalidate()
	{
		needsRedraw_ = true;

		Reset();
	}
	//---------------------------------------------------------------------------
	void Application::GuiRenderSurface::Draw()
	{
		if (needsRedraw_)
		{

		}

		RenderSurface::Draw();
	}
	//---------------------------------------------------------------------------

}
