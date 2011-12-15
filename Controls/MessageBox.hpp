#ifndef OSHGUI_CONTROLS_MESSAGEBOX_HPP
#define OSHGUI_CONTROLS_MESSAGEBOX_HPP

#include <functional>
#include <vector>
#include "Form.hpp"
#include "..\Misc\Strings.hpp"

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