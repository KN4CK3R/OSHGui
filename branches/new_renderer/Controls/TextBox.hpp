/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TEXTBOX_HPP
#define OSHGUI_TEXTBOX_HPP

#include "Control.hpp"
#include "../Misc/TextHelper.hpp"

namespace OSHGui
{
	/**
	 * Tritt ein, wenn sich der Wert der Text-Eigenschaft ändert.
	 */
	typedef Event<void(Control*)> TextChangedEvent;
	typedef EventHandler<void(Control*)> TextChangedEventHandler;

	/**
	 * Stellt ein Textfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT TextBox : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TextBox();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeI &size) override;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * \param font
		 */
		virtual void SetFont(const Drawing::FontPtr &font) override;
		/**
		 * Legt den Text fest.
		 *
		 * \param text
		 */
		virtual void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * \return der Text
		 */
		virtual const Misc::AnsiString& GetText() const;
		/**
		 * Legt das Zeichen fest, das bei der Kennworteingabe angezeigt wird.
		 *
		 * \param passwordChar
		 */
		virtual void SetPasswordChar(const Misc::AnsiChar &passwordChar);
		/**
		 * Ruft das Zeichen ab, das bei der Kennworteingabe angezeigt wird.
		 */
		virtual const Misc::AnsiChar& GetPasswordChar() const;
		/**
		 * Ruft das TextChangedEvent für das Steuerelement ab.
		 *
		 * \return textChangedEvent
		 */
		TextChangedEvent& GetTextChangedEvent();

		virtual void ShowCaret(bool showCaret);
		
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;
	
	protected:
		virtual void InjectTime(const Misc::DateTime &time) override;

		virtual void PopulateGeometry() override;

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual bool OnKeyPress(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
		virtual void OnTextChanged();

	private:
		static const Drawing::SizeI DefaultSize;
		static const Drawing::PointI DefaultTextOffset;

		void ResetCaretBlink();
		void PlaceCaret(int position);
			
		Misc::TextHelper textHelper;
		
		Misc::AnsiChar passwordChar;
		Misc::AnsiString realtext;

		Drawing::RectangleI textRect;
		Drawing::RectangleI caretRect;
		
		bool showCaret;
		bool drawCaret;
		Misc::TimeSpan blinkTime;
		Misc::DateTime nextBlinkTime;
		int caretPosition;
		int firstVisibleCharacter;

		TextChangedEvent textChangedEvent;
	};
}

#endif