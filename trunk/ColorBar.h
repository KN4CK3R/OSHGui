#ifndef __OSHGUI_COLORBAR_H__
#define __OSHGUI_COLORBAR_H__

#include <memory>
#include "Event\EventHandler.h"
#include "Control.h"
#include "Drawing\Color.h"
#include "Drawing\ITexture.h"

namespace OSHGui
{
	/**
	 * Tritt auf, wenn sich der Wert der SelectedIndex-Eigenschaft ändert.
	 */
	typedef EventHandler<void(Control*)> ColorChangeEvent;

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
		
		/**
		 * Ruft das ColorChangeEvent für das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangeEvent& GetColorChangeEvent();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
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
		void UpdateBars();
	
		bool drag[3];
		Drawing::Color color;
		std::vector<std::shared_ptr<Drawing::ITexture> > bars;
		std::vector<Drawing::Point> barSliders;

		ColorChangeEvent colorChangeEvent;
	};
}

#endif