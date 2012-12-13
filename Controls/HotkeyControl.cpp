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
		type = CONTROL_HOTKEYCONTROL;
	
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

		std::map<Key::Keys, Misc::AnsiString> hotkeyNames;
		{
			hotkeyNames[None] = EnumToString(None);
			hotkeyNames[LButton] = EnumToString(LButton);
			hotkeyNames[RButton] = EnumToString(RButton);
			hotkeyNames[Cancel] = EnumToString(Cancel);
			hotkeyNames[MButton] = EnumToString(MButton);
			hotkeyNames[XButton1] = EnumToString(XButton1);
			hotkeyNames[XButton2] = EnumToString(XButton2);
			hotkeyNames[Back] = EnumToString(Back);
			hotkeyNames[Tab] = EnumToString(Tab);
			hotkeyNames[LineFeed] = EnumToString(LineFeed);
			hotkeyNames[Clear] = EnumToString(Clear);
			hotkeyNames[Enter] = EnumToString(Enter);
			hotkeyNames[Return] = EnumToString(Return);
			hotkeyNames[ShiftKey] = EnumToString(ShiftKey);
			hotkeyNames[ControlKey] = EnumToString(ControlKey);
			hotkeyNames[Menu] = EnumToString(Menu);
			hotkeyNames[Pause] = EnumToString(Pause);
			hotkeyNames[CapsLock] = EnumToString(CapsLock);
			hotkeyNames[Capital] = EnumToString(Capital);
			hotkeyNames[KanaMode] = EnumToString(KanaMode);
			hotkeyNames[HanguelMode] = EnumToString(HanguelMode);
			hotkeyNames[HangulMode] = EnumToString(HangulMode);
			hotkeyNames[JunjaMode] = EnumToString(JunjaMode);
			hotkeyNames[FinalMode] = EnumToString(FinalMode);
			hotkeyNames[KanjiMode] = EnumToString(KanjiMode);
			hotkeyNames[HanjaMode] = EnumToString(HanjaMode);
			hotkeyNames[Escape] = EnumToString(Escape);
			hotkeyNames[IMEConvert] = EnumToString(IMEConvert);
			hotkeyNames[IMENonconvert] = EnumToString(IMENonconvert);
			hotkeyNames[IMEAceept] = EnumToString(IMEAceept);
			hotkeyNames[IMEAccept] = EnumToString(IMEAccept);
			hotkeyNames[IMEModeChange] = EnumToString(IMEModeChange);
			hotkeyNames[Space] = EnumToString(Space);
			hotkeyNames[Prior] = EnumToString(Prior);
			hotkeyNames[PageUp] = EnumToString(PageUp);
			hotkeyNames[Next] = EnumToString(Next);
			hotkeyNames[PageDown] = EnumToString(PageDown);
			hotkeyNames[End] = EnumToString(End);
			hotkeyNames[Home] = EnumToString(Home);
			hotkeyNames[Left] = EnumToString(Left);
			hotkeyNames[Up] = EnumToString(Up);
			hotkeyNames[Right] = EnumToString(Right);
			hotkeyNames[Down] = EnumToString(Down);
			hotkeyNames[Select] = EnumToString(Select);
			hotkeyNames[Print] = EnumToString(Print);
			hotkeyNames[Execute] = EnumToString(Execute);
			hotkeyNames[PrintScreen] = EnumToString(PrintScreen);
			hotkeyNames[Snapshot] = EnumToString(Snapshot);
			hotkeyNames[Insert] = EnumToString(Insert);
			hotkeyNames[Delete] = EnumToString(Delete);
			hotkeyNames[Help] = EnumToString(Help);
			hotkeyNames[D0] = EnumToString(D0);
			hotkeyNames[D1] = EnumToString(D1);
			hotkeyNames[D2] = EnumToString(D2);
			hotkeyNames[D3] = EnumToString(D3);
			hotkeyNames[D4] = EnumToString(D4);
			hotkeyNames[D5] = EnumToString(D5);
			hotkeyNames[D6] = EnumToString(D6);
			hotkeyNames[D7] = EnumToString(D7);
			hotkeyNames[D8] = EnumToString(D8);
			hotkeyNames[D9] = EnumToString(D9);
			hotkeyNames[A] = EnumToString(A);
			hotkeyNames[B] = EnumToString(B);
			hotkeyNames[C] = EnumToString(C);
			hotkeyNames[D] = EnumToString(D);
			hotkeyNames[E] = EnumToString(E);
			hotkeyNames[F] = EnumToString(F);
			hotkeyNames[G] = EnumToString(G);
			hotkeyNames[H] = EnumToString(H);
			hotkeyNames[I] = EnumToString(I);
			hotkeyNames[J] = EnumToString(J);
			hotkeyNames[K] = EnumToString(K);
			hotkeyNames[L] = EnumToString(L);
			hotkeyNames[M] = EnumToString(M);
			hotkeyNames[N] = EnumToString(N);
			hotkeyNames[O] = EnumToString(O);
			hotkeyNames[P] = EnumToString(P);
			hotkeyNames[Q] = EnumToString(Q);
			hotkeyNames[R] = EnumToString(R);
			hotkeyNames[S] = EnumToString(S);
			hotkeyNames[T] = EnumToString(T);
			hotkeyNames[U] = EnumToString(U);
			hotkeyNames[V] = EnumToString(V);
			hotkeyNames[W] = EnumToString(W);
			hotkeyNames[X] = EnumToString(X);
			hotkeyNames[Y] = EnumToString(Y);
			hotkeyNames[Z] = EnumToString(Z);
			hotkeyNames[LWin] = EnumToString(LWin);
			hotkeyNames[RWin] = EnumToString(RWin);
			hotkeyNames[Apps] = EnumToString(Apps);
			hotkeyNames[Sleep] = EnumToString(Sleep);
			hotkeyNames[NumPad0] = EnumToString(NumPad0);
			hotkeyNames[NumPad1] = EnumToString(NumPad1);
			hotkeyNames[NumPad2] = EnumToString(NumPad2);
			hotkeyNames[NumPad3] = EnumToString(NumPad3);
			hotkeyNames[NumPad4] = EnumToString(NumPad4);
			hotkeyNames[NumPad5] = EnumToString(NumPad5);
			hotkeyNames[NumPad6] = EnumToString(NumPad6);
			hotkeyNames[NumPad7] = EnumToString(NumPad7);
			hotkeyNames[NumPad8] = EnumToString(NumPad8);
			hotkeyNames[NumPad9] = EnumToString(NumPad9);
			hotkeyNames[Multiply] = EnumToString(Multiply);
			hotkeyNames[Add] = EnumToString(Add);
			hotkeyNames[Separator] = EnumToString(Separator);
			hotkeyNames[Subtract] = EnumToString(Subtract);
			hotkeyNames[Decimal] = EnumToString(Decimal);
			hotkeyNames[Divide] = EnumToString(Divide);
			hotkeyNames[F1] = EnumToString(F1);
			hotkeyNames[F2] = EnumToString(F2);
			hotkeyNames[F3] = EnumToString(F3);
			hotkeyNames[F4] = EnumToString(F4);
			hotkeyNames[F5] = EnumToString(F5);
			hotkeyNames[F6] = EnumToString(F6);
			hotkeyNames[F7] = EnumToString(F7);
			hotkeyNames[F8] = EnumToString(F8);
			hotkeyNames[F9] = EnumToString(F9);
			hotkeyNames[F10] = EnumToString(F10);
			hotkeyNames[F11] = EnumToString(F11);
			hotkeyNames[F12] = EnumToString(F12);
			hotkeyNames[F13] = EnumToString(F13);
			hotkeyNames[F14] = EnumToString(F14);
			hotkeyNames[F15] = EnumToString(F15);
			hotkeyNames[F16] = EnumToString(F16);
			hotkeyNames[F17] = EnumToString(F17);
			hotkeyNames[F18] = EnumToString(F18);
			hotkeyNames[F19] = EnumToString(F19);
			hotkeyNames[F20] = EnumToString(F20);
			hotkeyNames[F21] = EnumToString(F21);
			hotkeyNames[F22] = EnumToString(F22);
			hotkeyNames[F23] = EnumToString(F23);
			hotkeyNames[F24] = EnumToString(F24);
			hotkeyNames[NumLock] = EnumToString(NumLock);
			hotkeyNames[Scroll] = EnumToString(Scroll);
			hotkeyNames[LShiftKey] = EnumToString(LShiftKey);
			hotkeyNames[RShiftKey] = EnumToString(RShiftKey);
			hotkeyNames[LControlKey] = EnumToString(LControlKey);
			hotkeyNames[RControlKey] = EnumToString(RControlKey);
			hotkeyNames[LMenu] = EnumToString(LMenu);
			hotkeyNames[RMenu] = EnumToString(RMenu);
			hotkeyNames[BrowserBack] = EnumToString(BrowserBack);
			hotkeyNames[BrowserForward] = EnumToString(BrowserForward);
			hotkeyNames[BrowserRefresh] = EnumToString(BrowserRefresh);
			hotkeyNames[BrowserStop] = EnumToString(BrowserStop);
			hotkeyNames[BrowserSearch] = EnumToString(BrowserSearch);
			hotkeyNames[BrowserFavorites] = EnumToString(BrowserFavorites);
			hotkeyNames[BrowserHome] = EnumToString(BrowserHome);
			hotkeyNames[VolumeMute] = EnumToString(VolumeMute);
			hotkeyNames[VolumeDown] = EnumToString(VolumeDown);
			hotkeyNames[VolumeUp] = EnumToString(VolumeUp);
			hotkeyNames[MediaNextTrack] = EnumToString(MediaNextTrack);
			hotkeyNames[MediaPreviousTrack] = EnumToString(MediaPreviousTrack);
			hotkeyNames[MediaStop] = EnumToString(MediaStop);
			hotkeyNames[MediaPlayPause] = EnumToString(MediaPlayPause);
			hotkeyNames[LaunchMail] = EnumToString(LaunchMail);
			hotkeyNames[SelectMedia] = EnumToString(SelectMedia);
			hotkeyNames[LaunchApplication1] = EnumToString(LaunchApplication1);
			hotkeyNames[LaunchApplication2] = EnumToString(LaunchApplication2);
			hotkeyNames[Oem1] = EnumToString(Oem1);
			hotkeyNames[OemSemicolon] = EnumToString(OemSemicolon);
			hotkeyNames[Oemplus] = EnumToString(Oemplus);
			hotkeyNames[Oemcomma] = EnumToString(Oemcomma);
			hotkeyNames[OemMinus] = EnumToString(OemMinus);
			hotkeyNames[OemPeriod] = EnumToString(OemPeriod);
			hotkeyNames[OemQuestion] = EnumToString(OemQuestion);
			hotkeyNames[Oem2] = EnumToString(Oem2);
			hotkeyNames[Oemtilde] = EnumToString(Oemtilde);
			hotkeyNames[Oem3] = EnumToString(Oem3);
			hotkeyNames[Oem4] = EnumToString(Oem4);
			hotkeyNames[OemOpenBrackets] = EnumToString(OemOpenBrackets);
			hotkeyNames[OemPipe] = EnumToString(OemPipe);
			hotkeyNames[Oem5] = EnumToString(Oem5);
			hotkeyNames[Oem6] = EnumToString(Oem6);
			hotkeyNames[OemCloseBrackets] = EnumToString(OemCloseBrackets);
			hotkeyNames[Oem7] = EnumToString(Oem7);
			hotkeyNames[OemQuotes] = EnumToString(OemQuotes);
			hotkeyNames[Oem8] = EnumToString(Oem8);
			hotkeyNames[Oem102] = EnumToString(Oem102);
			hotkeyNames[OemBackslash] = EnumToString(OemBackslash);
			hotkeyNames[ProcessKey] = EnumToString(ProcessKey);
			hotkeyNames[Packet] = EnumToString(Packet);
			hotkeyNames[Attn] = EnumToString(Attn);
			hotkeyNames[Crsel] = EnumToString(Crsel);
			hotkeyNames[Exsel] = EnumToString(Exsel);
			hotkeyNames[EraseEof] = EnumToString(EraseEof);
			hotkeyNames[Play] = EnumToString(Play);
			hotkeyNames[Zoom] = EnumToString(Zoom);
			hotkeyNames[NoName] = EnumToString(NoName);
			hotkeyNames[Pa1] = EnumToString(Pa1);
			hotkeyNames[OemClear] = EnumToString(OemClear);
		}

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
		else if (modifier != Key::None && hotkey != Key::None && (hotkey != Key::ShiftKey && hotkey != Key::Menu && hotkey != Key::ControlKey))
		{
			auto modifierText = ModifierToString(modifier);
			auto hotkeyText = hotkeyNames[hotkey];

			TextBox::SetText(modifierText + Misc::AnsiString(" + ") + hotkeyText);
		}
		else if (hotkey != Key::None && (hotkey != Key::ShiftKey && hotkey != Key::Menu && hotkey != Key::ControlKey))
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
		KeyEventArgs args(keyboard);
		switch (args.GetKeyCode())
		{
			case Key::Delete:
			case Key::Back:
				ClearHotkey();
				break;
			default:
				KeyEventArgs args(keyboard);
				hotkey = args.GetKeyCode();
				modifier = args.GetModifier();
				HotkeyToText();
				break;
		}

		return false;
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