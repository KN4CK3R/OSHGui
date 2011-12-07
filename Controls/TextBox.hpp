#ifndef OSHGUI_TEXTBOX_HPP_
#define OSHGUI_TEXTBOX_HPP_

#include "Control.hpp"
#include "..\Misc\TextHelper.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT TextBox : public Control
	{
	public:
		using Control::SetSize;

		static const Drawing::Size DefaultSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TextBox();
		virtual ~TextBox();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Gibt den Text zurück.
		 *
		 * @return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Ruft das TextChangedEvent für das Steuerelement ab.
		 *
		 * @return textChangedEvent
		 */
		TextChangedEvent& GetTextChangedEvent();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		void Render(Drawing::IRenderer *renderer);
	
	private:
		static const Drawing::Point DefaultTextOffset;

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