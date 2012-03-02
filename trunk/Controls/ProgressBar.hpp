/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_PROGRESSBAR_HPP
#define OSHGUI_PROGRESSBAR_HPP

#include "Control.hpp"
#include "Panel.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Statusanzeige-Steuerlement dar.
	 */
	class OSHGUI_EXPORT ProgressBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		ProgressBar();
		virtual ~ProgressBar();
	
		/**
		 * Legt den minimalen Wert fest.
		 *
		 * @param min
		 */
		void SetMin(int min);
		/**
		 * Gibt den minimalen Wert zur�ck.
		 *
		 * @return der minimale Wert
		 */
		int GetMin() const;
		/**
		 * Legt den maximalen Wert fest.
		 *
		 * @param max
		 */
		void SetMax(int max);
		/**
		 * Gibt den maximalen Wert zur�ck.
		 *
		 * @return der maximale Wert
		 */
		int GetMax() const;
		/**
		 * Legt den aktuellen Wert fest.
		 *
		 * @param value
		 */
		void SetValue(int value);
		/**
		 * Ruft den aktuellen Wert ab.
		 *
		 * @return der Wert
		 */
		int GetValue() const;
		/**
		 * Legt die Farbe des Fortschrittsbalken fest.
		 *
		 * @param color
		 */
		void SetBarColor(const Drawing::Color &color);
		/**
		 * Gibt die Farbe des Fortschrittsbalken zur�ck.
		 *
		 * @return die Farbe
		 */
		const Drawing::Color& GetBarColor() const;

		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
	
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	private:
		static const Drawing::Size DefaultSize;

		void Adjust();
	
		Drawing::Color barColor;
		int value;
		int min;
		int max;
	};
}

#endif