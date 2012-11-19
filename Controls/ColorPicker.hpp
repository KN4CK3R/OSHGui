/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COLORPICKER_HPP
#define OSHGUI_COLORPICKER_HPP

#include <memory>
#include "Control.hpp"
#include "../Drawing/ITexture.hpp"

namespace OSHGui
{
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
		virtual ~ColorPicker();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size) override;
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
		Drawing::Color GetColorAtPoint(const Drawing::Point &point) const;

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
		virtual bool Intersect(const Drawing::Point &point) const override;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
		
	private:
		static const Drawing::Size DefaultSize;

		void CreateGradientTexture();
		void CalculateColorCursorLocation();

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
	
		Drawing::Color color;
		Drawing::Point colorCursorLocation;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangedEvent colorChangedEvent;

		bool drag;
	};
}

#endif