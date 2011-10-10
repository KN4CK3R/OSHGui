#ifndef OSHGUI_CONTROLS_MESSAGEBOX_H_
#define OSHGUI_CONTROLS_MESSAGEBOX_H_

#include <functional>
#include <vector>
#include "Form.h"
#include "..\Misc\Strings.h"

namespace OSHGui
{
	/**
	 * Gibt Konstanten an, die definieren, welche Schaltflächen der MessageBox angezeigt werden.
	 */
	enum MessageBoxButtons
	{
		/**
		 * Das Meldungsfeld enthält die Schaltfläche OK.
		 */
        ButtonOK,
        /**
		 * Das Meldungsfeld enthält die Schaltflächen OK und Abbrechen.
		 */
        ButtonOKCancel,
		/**
		 * Das Meldungsfeld enthält die Schaltflächen Abbrechen, Wiederholen und Ignorieren.
		 */
        ButtonAbortRetryIgnore,
		/**
		 * Das Meldungsfeld enthält die Schaltflächen Ja und Nein.
		 */
        ButtonYesNo,
		/**
		 * Das Meldungsfeld enthält die Schaltflächen Ja, Nein und Abbrechen.
		 */
        ButtonYesNoCancel,
        /**
		 * Das Meldungsfeld enthält die Schaltflächen Wiederholen und Abbrechen.
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
			void AddButton(const std::vector<Misc::UnicodeString> &label, const std::vector<std::function<void(Control *control)> > &event);

		public:
			MessageBoxForm(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons);
		};
	};
}

#endif