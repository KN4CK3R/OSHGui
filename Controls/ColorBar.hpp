/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COLORBAR_HPP
#define OSHGUI_COLORBAR_HPP

#include <memory>
#include "Control.hpp"
#include "../Drawing/ITexture.hpp"

namespace OSHGui
{
	/**
	 * Wird zum Auswählen einer Farbe verwendet.
	 */
	class OSHGUI_EXPORT ColorBar : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ColorBar();
		virtual ~ColorBar();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size) override;
		/**
		 * Legt die ausgewählte Farbe fest.
		 *
		 * @param color die Farbe
		 */
		void SetColor(Drawing::Color color);
		/**
		 * Ruft die ausgewählte Farbe ab.
		 *
		 * @return color
		 */
		Drawing::Color GetColor() const;
		/**
		 * Ruft das ColorChangeEvent für das Steuerelement ab.
		 *
		 * @return colorChangeEvent
		 */
		ColorChangedEvent& GetColorChangedEvent();
		
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
		
	protected:
		static const Drawing::Size DefaultSize;
		static const Drawing::Size DefaultBarSize;

		void CreateBarTexture(int index);
		void UpdateBars();

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
	
		int barIndex;
		bool drag[3];
		Drawing::Color color;
		std::vector<std::shared_ptr<Drawing::ITexture> > bars;
		std::vector<Drawing::Point> barSliderLocation;
		std::vector<Drawing::Point> barSliderAbsoluteLocation;

		ColorChangedEvent colorChangedEvent;
	};
}

#endif