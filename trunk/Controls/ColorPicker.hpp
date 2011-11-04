#ifndef OSHGUI_COLORPICKER_HPP_
#define OSHGUI_COLORPICKER_HPP_

#include <memory>
#include "Control.hpp"
#include "..\Drawing\Color.hpp"
#include "..\Drawing\ITexture.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Ausw�hlen einer Farbe verwendet.
	 */
	class OSHGUI_EXPORT ColorPicker : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ColorPicker(Control *parent);
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
		ColorChangedEvent& GetColorChangedEvent();
		
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

		ColorChangedEvent colorChangedEvent;
	};
}

#endif