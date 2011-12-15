#ifndef OSHGUI_COLORBAR_HPP
#define OSHGUI_COLORBAR_HPP

#include <memory>
#include "Control.hpp"
#include "..\Drawing\ITexture.hpp"

namespace OSHGui
{
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
		virtual ~ColorBar();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die ausgewählte Farbe fest.
		 *
		 * @param color die Farbe
		 */
		void SetColor(const Drawing::Color &color);
		/**
		 * Ruft die ausgewählte Farbe ab.
		 *
		 * @return color
		 */
		const Drawing::Color& GetColor() const;
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
		virtual bool Intersect(const Drawing::Point &point) const;
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		static const Drawing::Size DefaultSize;
		static const Drawing::Size DefaultBarSize;

		void CreateBarTexture(int index);
		void UpdateBars();

		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseUp(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);
		virtual bool OnKeyDown(const KeyboardMessage &keyboard);
	
		int barIndex;
		bool drag[3];
		Drawing::Color color;
		std::vector<std::shared_ptr<Drawing::ITexture> > bars;
		std::vector<Drawing::Point> barSliderLocation;
		std::vector<Drawing::Point> barSliderAbsoluteLocation;

		ColorChangedEvent colorChangedEvent;
	};
}

#endif