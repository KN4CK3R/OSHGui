/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_LABEL_HPP
#define OSHGUI_LABEL_HPP

#include "Control.hpp"
#include "../Misc/TextHelper.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar.
	 */
	class OSHGUI_EXPORT Label : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Label();
		
		/**
		 * Legt den Text fest. Falls autoSize = true, wird die Gr��e automatisch angepasst.
		 *
		 * @param text
		 */
		void SetText(const Misc::AnsiString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * @return der Text
		 */
		const Misc::AnsiString& GetText() const;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font) override;
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;
				
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
		
	protected:
		Misc::TextHelper textHelper;
	};
}

#endif