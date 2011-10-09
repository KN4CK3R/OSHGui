#ifndef OSHGUI_CONTROLS_MESSAGEBOX_H_
#define OSHGUI_CONTROLS_MESSAGEBOX_H_

#include <functional>
#include "Form.h"
#include "..\Misc\Strings.h"

namespace OSHGui
{
	class Label;
	class Button;
	class PictureBox;

	/**
	 * Gibt Konstanten an, die definieren, welche Schaltfl‰chen der MessageBox angezeigt werden.
	 */
	enum MessageBoxButtons
	{
		/**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰che OK.
		 */
        ButtonOK,
        /**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰chen OK und Abbrechen.
		 */
        ButtonOKCancel,
		/**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰chen Abbrechen, Wiederholen und Ignorieren.
		 */
        ButtonAbortRetryIgnore,
		/**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰chen Ja und Nein.
		 */
        ButtonYesNo,
		/**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰chen Ja, Nein und Abbrechen.
		 */
        ButtonYesNoCancel,
        /**
		 * Das Meldungsfeld enth‰lt die Schaltfl‰chen Wiederholen und Abbrechen.
		 */
        ButtonRetryCancel
	};

	/**
	 * Gibt Konstanten an, die definieren, welche Informationen angezeigt werden.
	 */
    enum MessageBoxIcon
    {
        /**
         * Das Meldungsfeld enth‰lt keine Symbole.
		 */
        IconNone = 0,
        /**
		 * Das Meldungsfeld enth‰lt ein Symbol, das aus einem weiﬂen X in einem Kreis mit rotem Hintergrund besteht.
         */
        IconError = 1,
        /**
		 * Das Meldungsfeld enth‰lt ein Symbol, das aus einem weiﬂen X in einem Kreis mit rotem Hintergrund besteht.
         */
        IconStop = 1,
        /**
         * Das Meldungsfeld enth‰lt ein Symbol, das aus einem Ausrufezeichen in einem Dreieck mit gelbem Hintergrund besteht.
		 */
        IconExclamation = 2,
        /**
         * Das Meldungsfeld enth‰lt ein Symbol, das aus einem Ausrufezeichen in einem Dreieck mit gelbem Hintergrund besteht.
		 */
        IconWarning = 2,
        /**
         * Das Meldungsfeld enth‰lt ein Symbol, das aus dem Kleinbuchstaben i in einem Kreis besteht.
         */
        IconInformation = 3
    };

	class MessageBox
	{
	public:
		static void Show(const Misc::UnicodeString &text);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon);
		static void Show(const Misc::UnicodeString &text, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, std::function<void(DialogResult result)> closeFunction);
		static void Show(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon, std::function<void(DialogResult result)> closeFunction);

	private:
		class MessageBoxForm : public Form
		{
		private:
			DialogResult defaultResult;

			void InitializeComponent(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon);

		public:
			MessageBoxForm(const Misc::UnicodeString &text, const Misc::UnicodeString &caption, MessageBoxButtons buttons, MessageBoxIcon icon);
		};
	};
}

#endif