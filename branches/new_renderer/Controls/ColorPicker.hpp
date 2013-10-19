/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COLORPICKER_HPP
#define OSHGUI_COLORPICKER_HPP

#include <memory>
#include "Control.hpp"
#include "../Drawing/Texture.hpp"

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
	class OSHGUI_EXPORT ColorPicker : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ColorPicker();
		
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
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * @param x
		 * @param y
		 * @return color
		 */
		Drawing::Color GetColorAtPoint(int x, int y) const;
		/**
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * @param point
		 * @return color
		 */
		Drawing::Color GetColorAtPoint(const Drawing::PointF &point) const;

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

	private:
		static const Drawing::SizeF DefaultSize;

		void CreateGradientTexture();
		void CalculateColorCursorLocation();

		virtual void PopulateGeometry() override;

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
	
		Drawing::Color color;
		Drawing::PointF colorCursorLocation;
		Drawing::TexturePtr gradient;

		ColorChangedEvent colorChangedEvent;

		bool drag;
	};
}

#endif