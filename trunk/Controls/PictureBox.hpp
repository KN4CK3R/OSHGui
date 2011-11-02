#ifndef OSHGUI_PICTUREBOX_HPP_
#define OSHGUI_PICTUREBOX_HPP_

#include <memory>
#include "Control.hpp"
#include "..\Drawing\ITexture.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Anzeigen eines Bildes verwendet.
	 */
	class OSHGUI_EXPORT PictureBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		PictureBox(Control *parent);
		virtual ~PictureBox();
		
		/**
		 * Legt das in der PictureBox dargestellte Bild fest.
		 *
		 * @param image das Bild
		 */
		void SetImage(const std::shared_ptr<Drawing::ITexture> &image);
		/**
		 * Ruft das in der PictureBox dargestellte Bild ab.
		 *
		 * @return das Bild
		 */
		std::shared_ptr<Drawing::ITexture> GetImage() const;
		
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
		std::shared_ptr<Drawing::ITexture> image;
	};
}

#endif