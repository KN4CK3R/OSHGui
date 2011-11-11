#ifndef OSHGUI_TEXTBOX_HPP_
#define OSHGUI_TEXTBOX_HPP_

#include "Control.hpp"
#include "..\Misc\TextHelper.hpp"

#define TEXTBOX_DEFAULT_HEIGHT 24

namespace OSHGui
{
	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT TextBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TextBox(Control *parent);
		virtual ~TextBox();
		
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::UnicodeString &text);
		/**
		 * Gibt den Text zurück.
		 *
		 * @return der Text
		 */
		const Misc::UnicodeString& GetText() const;
		/**
		 * Ruft das TextChangedEvent für das Steuerelement ab.
		 *
		 * @return textChangedEvent
		 */
		TextChangedEvent& GetTextChangedEvent();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		
		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
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

		TextChangedEvent textChangedEvent;
	};
}

#endif