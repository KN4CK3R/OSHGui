#ifndef OSHGUI_TEXTBOX_H_
#define OSHGUI_TEXTBOX_H_

#include "Control.h"
#include "..\Misc\TextHelper.h"

#define TEXTBOX_DEFAULT_HEIGHT 24

namespace OSHGui
{
	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class TextBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TextBox(Control *parent = 0);
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
		const Misc::UnicodeString& GetText() const;
		/**
		 * Ruft das KeyDownEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		/**
		 * Ruft das TextChangedEvent f�r das Steuerelement ab.
		 *
		 * @return textChangedEvent
		 */
		TextChangedEvent& GetTextChangedEvent();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
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
		void ResetCaretBlink();
		void PlaceCaret(int position);
			
		Misc::TextHelper textHelper;
		
		Drawing::Rectangle textRect,
						   caretRect;
		
		bool showCaret;
		Misc::TimeSpan blinkTime;
		Misc::DateTime nextBlinkTime;
		int caretPosition,
			firstVisibleCharacter;

		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
		TextChangedEvent textChangedEvent;
	};
}

#endif