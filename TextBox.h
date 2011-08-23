#ifndef __OSHGUI_TEXTBOX_H__
#define __OSHGUI_TEXTBOX_H__

#include "Control.h"
#include "Misc\TextHelper.h"

#define TEXTBOX_DEFAULT_HEIGHT 24

namespace OSHGui
{
	/**
	 * Diese Klasse repr�sentiert eine TextBox.
	 */
	class TextBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TextBox(Control *parent = NULL);
		virtual ~TextBox();
		
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
		const Misc::UnicodeString& GetText();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus();
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * L�scht den Text.
		 *
		 * @param text
		 */
		void ClearText();

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
		Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);
	
	protected:
		void ResetCaretBlink();

		void PlaceCaret(int position);
		
		void PasteFromClipboard();
		
		Misc::TextHelper textHelper;
		
		Drawing::Rectangle textRect,
						   caretRect;
		
		bool showCaret;
		double blinkTime,
			   lastBlinkTime;
		int caretPosition,
			firstVisibleCharacter;
	};
}

#endif