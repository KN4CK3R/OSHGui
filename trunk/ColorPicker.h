#ifndef __OSHGUI_COLORPICKER_H__
#define __OSHGUI_COLORPICKER_H__

#include <memory>
#include "Control.h"
#include "Drawing\Color.h"
#include "Drawing\ITexture.h"

namespace OSHGui
{
	/**
	 * Wird zum Auswählen einer Farbe verwendet.
	 */
	class ColorPicker : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ColorPicker(Control *parent = 0);
		virtual ~ColorPicker();
		
		Drawing::Color GetColor() const;
		Drawing::Color GetColorAtPoint(int x, int y) const;
		Drawing::Color GetColorAtPoint(const Drawing::Point &point) const;

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
		virtual void SetMouseOver(bool mouseOver);

		void CreateGradientTexture();
	
		bool drag;
		Drawing::Color color;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangeEvent colorChangeEvent;
	};
}

#endif