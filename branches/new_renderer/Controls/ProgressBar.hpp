/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	
		/**
		 * Legt den minimalen Wert fest.
		 *
		 * @param min
		 */
		void SetMin(int min);
		/**
		 * Gibt den minimalen Wert zurück.
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
		 * Gibt den maximalen Wert zurück.
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
		void SetBarColor(Drawing::Color color);
		/**
		 * Gibt die Farbe des Fortschrittsbalken zurück.
		 *
		 * @return die Farbe
		 */
		Drawing::Color GetBarColor() const;

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