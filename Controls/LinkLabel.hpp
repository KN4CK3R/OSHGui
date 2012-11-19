/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_LINKLABEL_HPP
#define OSHGUI_LINKLABEL_HPP

#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Label-Steuerelement dar, das Links anzeigen kann.
	 */
	class OSHGUI_EXPORT LinkLabel : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		LinkLabel();
		virtual ~LinkLabel();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
	};
}

#endif