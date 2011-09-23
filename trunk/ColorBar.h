#ifndef __OSHGUI_COLORBAR_H__
#define __OSHGUI_COLORBAR_H__

#include <memory>
#include "Control.h"
#include "Drawing\Color.h"
#include "Drawing\ITexture.h"

namespace OSHGui
{
	typedef EventHandler<void(Control*)> ColorChangeEventHandler;

	/**
	 * Wird zum Auswählen einer Farbe verwendet.
	 */
	class ColorBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ColorBar(Control *parent = 0);
		virtual ~ColorBar();
		
		void SetColor(Drawing::Color color);
		Drawing::Color GetColor() const;
		
		ColorChangeEventHandler& GetColorChangeEventHandler();
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird außerdem für alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void CreateBarTexture(int index);
	
		bool drag[3];
		Drawing::Color color;
		std::vector<std::shared_ptr<Drawing::ITexture>> bars;
		std::vector<Drawing::Rectangle> barRects;
		std::vector<Drawing::Point> barSliders;

		ColorChangeEventHandler colorChangeEventHandler;
	};
}

#endif