/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_BUTTON_HPP
#define OSHGUI_BUTTON_HPP

#include "Control.hpp"
#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Schaltfl�chen-Steuerelement dar.
	 */
	class OSHGUI_EXPORT Button : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		Button();

		/**
		 * Legt fest, ob sich die Gr��e des Steuerelements automatisch an dessen Inhalt anpasst.
		 *
		 * \param autoSize
		 */
		virtual void SetAutoSize(bool autoSize) override;
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeI &size) override;
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Gr��e automatisch angepasst.
		 *
		 * \param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * \return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * \param font
		 */
		virtual void SetFont(const Drawing::FontPtr &font) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(const Drawing::Color &color) override;
		
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		virtual void DrawSelf(Drawing::RenderContext &context, Skins::Base &skin) override;

	protected:
		virtual void CalculateLabelLocation();
		
		virtual void PopulateGeometry(Skins::Base &skin);

		virtual bool OnKeyUp(const KeyboardMessage &keyboard) override;

		std::unique_ptr<Label> label_;

	private:
		static const Drawing::SizeI DefaultSize;
		static const Drawing::PointI DefaultLabelOffset;
	};
}

#endif