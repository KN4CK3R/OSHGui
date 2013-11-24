/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_PANEL_HPP
#define OSHGUI_PANEL_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Gruppieren von Auflistungen von Steuerelementen verwendet.
	 */
	class OSHGUI_EXPORT Panel : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Panel();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;

	protected:
		virtual void PopulateGeometry() override;

	private:
		static const Drawing::SizeF DefaultSize;
	};
}

#endif