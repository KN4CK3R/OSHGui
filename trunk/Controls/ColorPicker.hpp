#ifndef OSHGUI_COLORPICKER_HPP_
#define OSHGUI_COLORPICKER_HPP_

#include <memory>
#include "Control.hpp"
#include "..\Drawing\ITexture.hpp"

namespace OSHGui
{
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
		virtual ~ColorPicker();
		
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die ausgew�hlte Farbe fest.
		 *
		 * @param color die Farbe
		 */
		void SetColor(const Drawing::Color &color);
		/**
		 * Ruft die ausgew�hlte Farbe ab.
		 *
		 * @return color
		 */
		const Drawing::Color& GetColor() const;
		/**
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * @param x
		 * @param y
		 * @return color
		 */
		const Drawing::Color& GetColorAtPoint(int x, int y) const;
		/**
		 * Ruft die Farbe an einem bestimmten Punkt ab.
		 *
		 * @param point
		 * @return color
		 */
		const Drawing::Color& GetColorAtPoint(const Drawing::Point &point) const;

		/**
		 * Ruft das ColorChangeEvent f�r das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangedEvent& GetColorChangedEvent() const;
		
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

		void CreateGradientTexture();
		void CalculateColorCursorLocation();

		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);
	
		Drawing::Color color;
		Drawing::Point colorCursorLocation;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangedEvent colorChangedEvent;

		bool drag;
	};
}

#endif