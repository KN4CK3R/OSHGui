#ifndef OSHGUI_CONTROLS_MESSAGEBOX_H_
#define OSHGUI_CONTROLS_MESSAGEBOX_H_

#include <functional>
#include "Form.h"
#include "..\Misc\Strings.h"

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

	class MessageBox
	{
	public:
		static void Show(const Misc::UnicodeString &text);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons);
		static void Show(const Misc::UnicodeString &text, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction);

	private:
		class MessageBoxForm : public Form
		{
		private:
			void InitializeComponent(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons);

		public:
			MessageBoxForm(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons);
		};
	};
}

#endif