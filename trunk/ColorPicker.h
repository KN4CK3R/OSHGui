#ifndef __OSHGUI_COLORPICKER_H__
#define __OSHGUI_COLORPICKER_H__

#include <memory>
#include "Control.h"
#include "Drawing\Color.h"
#include "Drawing\ITexture.h"

namespace OSHGui
{
	/**
	 * Wird zum Ausw�hlen einer Farbe verwendet.
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
		 * Ruft das ColorChangeEvent f�r das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangeEvent& GetColorChangeEvent();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
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
		virtual IEvent::NextEventTypes ProcessEvent(IEvent *event);
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
		Drawing::Point colorPosition;
		std::shared_ptr<Drawing::ITexture> gradient;

		ColorChangeEvent colorChangeEvent;
	};
}

#endif