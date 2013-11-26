/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COLORPICKER_HPP
#define OSHGUI_COLORPICKER_HPP

#include <memory>
#include "Control.hpp"
#include "../Drawing/Image.hpp"

namespace OSHGui
{
	/**
	 * Tritt ein, wenn sich der Wert der Color-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*, Drawing::Color &color)> ColorChangedEvent;
	typedef EventHandler<void(Control*, Drawing::Color &color)> ColorChangedEventHandler;

	/**
	 * Wird zum Ausw�hlen einer Farbe verwendet.
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
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt die ausgew�hlte Farbe fest.
		 *
		 * \param color die Farbe
		 */
		void SetColor(Drawing::Color color);
		/**
		 * Ruft die ausgew�hlte Farbe ab.
		 *
		 * \return color
		 */
		Drawing::Color GetColor() const;
		/**
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * \param x
		 * \param y
		 * \return color
		 */
		Drawing::Color GetColorAtPoint(int x, int y) const;
		/**
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * \param point
		 * \return color
		 */
		Drawing::Color GetColorAtPoint(const Drawing::PointF &point) const;

		/**
		 * Ruft das ColorChangeEvent f�r das Steuerelement ab.
		 *
		 * \return colorChangeEvent
		 */
		ColorChangedEvent& GetColorChangedEvent();

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
		Drawing::ImagePtr gradient;

		ColorChangedEvent colorChangedEvent;

		bool drag;
	};
}

#endif