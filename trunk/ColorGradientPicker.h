#ifndef __OSHGUI_COLORGRADIENTPICKER_H__
#define __OSHGUI_COLORGRADIENTPICKER_H__

#include <memory>
#include "Control.h"
#include "Drawing\Color.h"
#include "Drawing\ITexture.h"

namespace OSHGui
{
	typedef EventHandler<void(Control*)> ColorChangeEventHandler;

	/**
	 * Wird zum Ausw�hlen einer Farbe verwendet.
	 */
	class ColorGradientPicker : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ColorGradientPicker(Control *parent = 0);
		virtual ~ColorGradientPicker();
		
		Drawing::Color GetColor() const;
		Drawing::Color GetColorAtPoint(int x, int y) const;
		Drawing::Color GetColorAtPoint(const Drawing::Point &point) const;

		ColorChangeEventHandler& GetColorChangeEventHandler();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void CreateGradientTexture();
	
		bool drag;
		Drawing::Color color;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangeEventHandler colorChangeEventHandler;
	};
}

#endif