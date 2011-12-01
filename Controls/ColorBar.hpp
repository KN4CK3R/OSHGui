#ifndef OSHGUI_COLORBAR_HPP_
#define OSHGUI_COLORBAR_HPP_

#include <memory>
#include "Control.hpp"
#include "..\Drawing\Color.hpp"
#include "..\Drawing\ITexture.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Auswählen einer Farbe verwendet.
	 */
	class OSHGUI_EXPORT ColorBar : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ColorBar(Control *parent);
		virtual ~ColorBar();
		
		void SetColor(Drawing::Color color);
		Drawing::Color GetColor() const;
		/**
		 * Ruft das ColorChangeEvent für das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangedEvent& GetColorChangedEvent();
		/**
		 * Ruft das KeyDownEvent für das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyUpEvent für das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		
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
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Rechnet die Position des angegeben Bildschirmpunkts in Clientkoordinaten um.
		 *
		 * @param point
		 * @return der neue Punkt
		 */
		virtual const Drawing::Point PointToClient(const Drawing::Point &point) const;
		
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
		virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		void CreateBarTexture(int index);
		void UpdateBars();
	
		int barIndex;
		bool drag[3];
		Drawing::Color color;
		std::vector<std::shared_ptr<Drawing::ITexture> > bars;
		std::vector<Drawing::Point> barSliders;

		KeyDownEvent keyDownEvent;
		KeyUpEvent keyUpEvent;
		ColorChangedEvent colorChangedEvent;
	};
}

#endif