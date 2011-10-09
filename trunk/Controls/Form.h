#ifndef OSHGUI_FORM_H_
#define OSHGUI_FORM_H_

#include <functional>
#include "Control.h"
#include "..\Misc\TextHelper.h"

namespace OSHGui
{
	/**
	 * Gibt Bezeichner an, die den R�ckgabewert eines Dialogfelds angeben.
	 */
	enum DialogResult
	{
		/**
			* Der R�ckgabewert des Dialogfelds ist Nothing.
			*/
		None,
		/**
			* Der R�ckgabewert des Dialogfelds ist OK (�blicherweise von der Schaltfl�che OK gesendet).
			*/
		OK,
		/**
			* Der R�ckgabewert des Dialogfelds ist Cancel (�blicherweise von der Schaltfl�che Abbrechen gesendet).
			*/
		Cancel,
		/**
			* Der R�ckgabewert des Dialogfelds ist Abort (�blicherweise von der Schaltfl�che Abbrechen gesendet).
			*/
		Abort,
		/**
			* Der R�ckgabewert des Dialogfelds ist Retry (�blicherweise von der Schaltfl�che Wiederholen gesendet).
			*/
		Retry,
		/**
			* Der R�ckgabewert des Dialogfelds ist Ignore (�blicherweise von der Schaltfl�che Ignorieren gesendet).
			*/
		Ignore,
		/**
			* Der R�ckgabewert des Dialogfelds ist Yes (�blicherweise von der Schaltfl�che Ja gesendet).
			*/
		Yes,
		/**
			* Der R�ckgabewert des Dialogfelds ist No (�blicherweise von der Schaltfl�che Nein gesendet).
			*/
		No
	};

	/**
	 * Stellt ein Fenster dar, das die Benutzeroberfl�che bildet.
	 */
	class Form : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Form();
		virtual ~Form();

		bool IsModal() const;

		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::UnicodeString &text);
		/**
		 * Gibt den Text zur�ck.
		 *
		 * @return der Text
		 */
		const Misc::UnicodeString& GetText() const;
		/**
		 * Ruft das DialogResult f�r das Fenster ab.
		 *
		 * @return dialogResult
		 */
		DialogResult GetDialogResult() const;
		
		/**
		 * Zeigt die Form an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void Show(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance);
		/**
		 * Zeigt die Form modal an.
		 *
		 * @param instance die aktuelle Instanz dieser Form
		 * @param func diese Funktion wird ausgef�hrt, wenn die Form geschlossen wird (kann 0 sein)
		 */
		void ShowDialog(const std::shared_ptr<Form> &instance, const std::function<void()> &closeFunction);

		/**
		 * Schlie�t die Form.
		 */
		void Close();

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);

	protected:
		DialogResult dialogResult;

	private:
		Misc::TextHelper textHelper;
		Drawing::Rectangle captionBar,
						   closeRect;
		bool drag;
		bool isModal;

		std::weak_ptr<Form> instance;
	};
}

#endif