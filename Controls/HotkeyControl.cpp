/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <sstream>
#include <algorithm>
#include <iterator>
#include "HotkeyControl.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	HotkeyControl::HotkeyControl()		  
	{
		type = CONTROL_TEXTBOX;
	
		ApplyTheme(Application::Instance()->GetTheme());
		
		cursor = Cursors::Get(Cursors::Default);

		ClearHotkey();

		TextBox::ShowCaret(false);
	}
	//---------------------------------------------------------------------------
	HotkeyControl::~HotkeyControl()
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void HotkeyControl::SetText(const Misc::AnsiString &text)
	{

	}
	//---------------------------------------------------------------------------
	void HotkeyControl::SetHotkey(Key::Keys hotkey)
	{
		this->hotkey = hotkey;
		HotkeyToText();
	}
	//---------------------------------------------------------------------------
	Key::Keys HotkeyControl::GetHotkey() const
	{
		return hotkey;
	}
	//---------------------------------------------------------------------------
	void HotkeyControl::SetHotkeyModifier(Key::Keys modifier)
	{
		this->modifier = modifier;
		HotkeyToText();
	}
	//---------------------------------------------------------------------------
	Key::Keys HotkeyControl::GetHotkeyModifier() const
	{
		return modifier;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void HotkeyControl::ShowCaret(bool showCaret)
	{

	}
	//---------------------------------------------------------------------------
	void HotkeyControl::ClearHotkey()
	{
		hotkey = Key::None;
		modifier = Key::None;
		HotkeyToText();
	}
	//---------------------------------------------------------------------------
	void HotkeyControl::HotkeyToText()
	{
		#define EnumToString(x) #x

		using namespace Key;

		Misc::AnsiString hotkeyNames[] = {
			EnumToString(None),
			EnumToString(D0), EnumToString(D1), EnumToString(D2), EnumToString(D3), EnumToString(D4), EnumToString(D5), EnumToString(D6), EnumToString(D7), EnumToString(D8), EnumToString(D9),
			EnumToString(A), EnumToString(B), EnumToString(C), EnumToString(D), EnumToString(E), EnumToString(F), EnumToString(G), EnumToString(H), EnumToString(I), EnumToString(J), EnumToString(K), EnumToString(L), EnumToString(M), EnumToString(N), EnumToString(O), EnumToString(P), EnumToString(Q), EnumToString(R), EnumToString(S), EnumToString(T), EnumToString(U), EnumToString(V), EnumToString(W), EnumToString(X), EnumToString(Y), EnumToString(Z),
			EnumToString(Space), EnumToString(Back),
			EnumToString(Insert), EnumToString(Home), EnumToString(End), EnumToString(Delete), EnumToString(PageUp), EnumToString(PageDown),
			EnumToString(Tab),
			EnumToString(Return), EnumToString(Escape),
			EnumToString(Up), EnumToString(Right), EnumToString(Down), EnumToString(Left),
			EnumToString(Divide), EnumToString(Multiply), EnumToString(Substract), EnumToString(Add),
			EnumToString(F1), EnumToString(F2), EnumToString(F3), EnumToString(F4), EnumToString(F5), EnumToString(F6), EnumToString(F7), EnumToString(F8), EnumToString(F9), EnumToString(F10), EnumToString(F11), EnumToString(F12)
		};

		auto ModifierToString = [&hotkeyNames](Key::Keys modifier) -> Misc::AnsiString
		{
			std::vector<Misc::AnsiString> modifierNames;
			if ((modifier & Key::Control) == Key::Control) modifierNames.push_back("Control");
			if ((modifier & Key::Alt) == Key::Alt) modifierNames.push_back("Alt");
			if ((modifier & Key::Shift) == Key::Shift) modifierNames.push_back("Shift");

			std::stringstream s;
			for (auto i = 0; i < modifierNames.size(); ++i)
			{
				if (i > 0)
				{
					s << " + ";
				}
				s << modifierNames[i];
			}
			return s.str();
		};

		if (modifier == Key::None && hotkey == Key::None)
		{
			TextBox::SetText(hotkeyNames[hotkey]);
		}
		else if (modifier != Key::None && hotkey != Key::None)
		{
			auto modifierText = ModifierToString(modifier);
			auto hotkeyText = hotkeyNames[hotkey];

			TextBox::SetText(modifierText + Misc::AnsiString(" + ") + hotkeyText);
		}
		else if (hotkey != Key::None)
		{
			auto hotkeyText = hotkeyNames[hotkey];
			TextBox::SetText(hotkeyText);
		}
		else
		{
			auto modifierText = ModifierToString(modifier);
			TextBox::SetText(modifierText);
		}
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyDown(const KeyboardMessage &keyboard)
	{
		switch (keyboard.KeyCode)
		{
			case Key::Delete:
			case Key::Back:
				ClearHotkey();
				break;
			default:
				hotkey = keyboard.KeyCode;
				modifier = keyboard.Modifier;
				HotkeyToText();
				break;
		}

		return true;
	}
	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyPress(const KeyboardMessage &keyboard)
	{
		return true;
	}
	//---------------------------------------------------------------------------
	bool HotkeyControl::OnKeyUp(const KeyboardMessage &keyboard)
	{
		return true;
	}
	//---------------------------------------------------------------------------
}