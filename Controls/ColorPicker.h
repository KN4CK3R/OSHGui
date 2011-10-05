#ifndef OSHGUI_COLORPICKER_H_
#define OSHGUI_COLORPICKER_H_

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
		
		void SetColor(Drawing::Color color);
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
		virtual IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void CreateGradientTexture();
	
		bool drag;
		Drawing::Color color;
		Drawing::Point colorPosition;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangeEvent colorChangeEvent;
	};
}

#endif