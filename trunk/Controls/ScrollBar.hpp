/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_SCROLLBAR_HPP
#define OSHGUI_SCROLLBAR_HPP

#include "ContainerControl.hpp"

namespace OSHGui
{
	/**
	 * Implementiert die Basisfunktionen eines Schiebeleisten-Steuerelements.
	 */
	class OSHGUI_EXPORT ScrollBar : public ContainerControl
	{
	public:
		using ContainerControl::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ScrollBar();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color) override;
		/**
		 * Legt den aktuellen Wert des Bildlauffelds fets.
		 *
		 * @param value
		 */
		void SetValue(int value);
		/**
		 * Ruft den aktuellen Wert des Bildlauffelds ab.
		 *
		 * @return die Position
		 */
		int GetValue() const;
		/**
		 * Legt die Anzahl der scrollbaren Elemente fest.
		 *
		 * @param maximum
		 */
		void SetMaximum(int maximum);
		/**
		 * Ruft die Anzahl der scrollbaren Elemente ab.
		 *
		 * @return maximum
		 */
		int GetMaximum() const;
		/**
		 * Ruft das ScrollEvent für das Steuerelement ab.
		 *
		 * @return scrollEvent
		 */
		ScrollEvent& GetScrollEvent();

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;
		/**
		 * Verschiebt die ScrollBar zum Anfang.
		 */
		void ScrollToTop();
		/**
		 * Verschiebt die ScrollBar zum Ende.
		 */
		void ScrollToBottom();
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;
	
	protected:
		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
		virtual void OnMouseScroll(const MouseMessage &mouse) override;

	private:
		static const int MinimumSliderHeight = 25;
		static const Drawing::Size DefaultSize;

		void SetValueInternal(int value);

		bool drag;
		int value;
		float pixelsPerTick;
		int maximum;

		Drawing::Point trackLocation;
		Drawing::Point trackAbsoluteLocation;
		Drawing::Size trackSize;
		Drawing::Point sliderLocation;
		Drawing::Point sliderAbsoluteLocation;
		Drawing::Size sliderSize;

		ScrollEvent scrollEvent;

		class ScrollBarButton : public Control
		{
		public:
			using Control::SetSize;

			static const Drawing::Size DefaultButtonSize;

			ScrollBarButton(int direction);
			
			virtual void SetSize(const Drawing::Size &size) override;

			virtual bool Intersect(const Drawing::Point &point) const override;
			virtual void CalculateAbsoluteLocation() override;

			virtual void Render(Drawing::IRenderer *renderer) override;

		private:
			int direction;

			Drawing::Point iconLocation;
		};
		ScrollBarButton *upButton;
		ScrollBarButton *downButton;
	};
}

#endif