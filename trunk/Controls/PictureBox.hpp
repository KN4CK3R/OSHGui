/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_PICTUREBOX_HPP
#define OSHGUI_PICTUREBOX_HPP

#include <memory>
#include "Control.hpp"
#include "../Drawing/ITexture.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Anzeigen eines Bildes verwendet.
	 */
	class OSHGUI_EXPORT PictureBox : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		PictureBox();
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
		std::shared_ptr<Drawing::ITexture>& GetImage();
		/**
		 * Legt fest, ob das Bild gestrecht wird, wenn es kleiner ist, wie die Größe der PictureBox.
		 *
		 * @param strech
		 */
		void SetStretch(bool strech);
		/**
		 * Ruft ab, ob das Bild gestrecht wird, wenn es kleiner ist, wie die Größe der PictureBox.
		 *
		 * @return strech
		 */
		bool GetStretch() const;
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
		
	private:
		static const Drawing::Size DefaultSize;

		std::shared_ptr<Drawing::ITexture> image;
		bool stretch;
	};
}

#endif