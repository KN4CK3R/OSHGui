/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COLORBAR_HPP
#define OSHGUI_COLORBAR_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Tritt ein, wenn sich der Wert der Color-Eigenschaft ändert.
	 */
	typedef Event<void(Control*, Drawing::Color &color)> ColorChangedEvent;
	typedef EventHandler<void(Control*, Drawing::Color &color)> ColorChangedEventHandler;

	/**
	 * Wird zum Auswählen einer Farbe verwendet.
	 */
	class OSHGUI_EXPORT ColorBar : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ColorBar();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt die ausgewählte Farbe fest.
		 *
		 * @param color die Farbe
		 */
		void SetColor(Drawing::Color color);
		/**
		 * Ruft die ausgewählte Farbe ab.
		 *
		 * @return color
		 */
		Drawing::Color GetColor() const;
		/**
		 * Ruft das ColorChangeEvent für das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangedEvent& GetColorChangedEvent();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
		
	protected:
		static const Drawing::SizeF DefaultSize;
		static const Drawing::SizeF DefaultBarSize;

		void UpdateBars();

		virtual void PopulateGeometry() override;

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
	
		int barIndex;
		bool drag[3];
		Drawing::Color color;

		std::vector<Drawing::ColorRectangle> bars;
		std::vector<Drawing::PointF> barSliderLocation;
		std::vector<Drawing::PointF> barSliderAbsoluteLocation;

		ColorChangedEvent colorChangedEvent;
	};
}

#endif