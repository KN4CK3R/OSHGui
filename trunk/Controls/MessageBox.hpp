/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_CONTROLS_MESSAGEBOX_HPP
#define OSHGUI_CONTROLS_MESSAGEBOX_HPP

#include <functional>
#include <vector>
#include "Form.hpp"
#include "../Misc/Strings.hpp"

namespace OSHGui
{
	/**
	 * Gibt Konstanten an, die definieren, welche Schaltfl�chen der MessageBox angezeigt werden.
	 */
	enum MessageBoxButtons
	{
		/**
		 * Das Meldungsfeld enth�lt die Schaltfl�che OK.
		 */
        ButtonOK,
        /**
		 * Das Meldungsfeld enth�lt die Schaltfl�chen OK und Abbrechen.
		 */
        ButtonOKCancel,
		/**
		 * Das Meldungsfeld enth�lt die Schaltfl�chen Abbrechen, Wiederholen und Ignorieren.
		 */
        ButtonAbortRetryIgnore,
		/**
		 * Das Meldungsfeld enth�lt die Schaltfl�chen Ja und Nein.
		 */
        ButtonYesNo,
		/**
		 * Das Meldungsfeld enth�lt die Schaltfl�chen Ja, Nein und Abbrechen.
		 */
        ButtonYesNoCancel,
        /**
		 * Das Meldungsfeld enth�lt die Schaltfl�chen Wiederholen und Abbrechen.
		 */
        ButtonRetryCancel
	};

	class OSHGUI_EXPORT MessageBox
	{
	public:
		static void Show(const Misc::AnsiString &text);
		static void Show(const Misc::AnsiString &text, const Misc::AnsiString &caption);
		static void Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons);
		static void Show(const Misc::AnsiString &text, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction);

	private:
		class MessageBoxForm : public Form
		{
		private:
			void InitializeComponent(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons);
			void AddButtons(const std::vector<Misc::AnsiString> &label, const std::vector<ClickEventHandler> &eventHandler);

		public:
			MessageBoxForm(const Misc::AnsiString &text, const Misc::AnsiString &caption, MessageBoxButtons buttons);
		};
	};
}

#endif