/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_SCROLLBAR_HPP
#define OSHGUI_SCROLLBAR_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Tritt auf, wenn der ScrollBalken verschoben wird.
	 */
	typedef Event<void(Control*, ScrollEventArgs&)> ScrollEvent;
	typedef EventHandler<void(Control*, ScrollEventArgs&)> ScrollEventHandler;

	/**
	 * Implementiert die Basisfunktionen eines Schiebeleisten-Steuerelements.
	 */
	class OSHGUI_EXPORT ScrollBar : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ScrollBar();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(Drawing::Color color) override;
		/**
		 * Legt den aktuellen Wert des Bildlauffelds fets.
		 *
		 * \param value
		 */
		void SetValue(int value);
		/**
		 * Ruft den aktuellen Wert des Bildlauffelds ab.
		 *
		 * \return die Position
		 */
		int GetValue() const;
		/**
		 * Legt die Anzahl der scrollbaren Elemente fest.
		 *
		 * \param maximum
		 */
		void SetMaximum(int maximum);
		/**
		 * Ruft die Anzahl der scrollbaren Elemente ab.
		 *
		 * \return maximum
		 */
		int GetMaximum() const;
		/**
		 * Ruft das ScrollEvent für das Steuerelement ab.
		 *
		 * \return scrollEvent
		 */
		ScrollEvent& GetScrollEvent();

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
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
		
		virtual void DrawSelf(Drawing::RenderContext &context) override;

	protected:
		virtual void PopulateGeometry() override;

		virtual void OnMouseDown(const MouseMessage &mouse) override;
		virtual void OnMouseUp(const MouseMessage &mouse) override;
		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual void OnMouseMove(const MouseMessage &mouse) override;
		virtual void OnMouseScroll(const MouseMessage &mouse) override;

	private:
		static const int MinimumSliderHeight = 25;
		static const Drawing::SizeF DefaultSize;

		void SetValueInternal(int value);

		bool drag;
		int value;
		float pixelsPerTick;
		int maximum;

		Drawing::PointF trackLocation;
		Drawing::PointF trackAbsoluteLocation;
		Drawing::SizeF trackSize;
		Drawing::PointF sliderLocation;
		Drawing::PointF sliderAbsoluteLocation;
		Drawing::SizeF sliderSize;

		ScrollEvent scrollEvent;

		class ScrollBarButton : public Control
		{
		public:
			using Control::SetSize;

			static const Drawing::SizeF DefaultButtonSize;

			ScrollBarButton(int direction);
			
			virtual void SetSize(const Drawing::SizeF &size) override;

		protected:
			virtual void PopulateGeometry() override;

		private:
			int direction;

			Drawing::PointF iconLocation;
		};

		ScrollBarButton *upButton;
		ScrollBarButton *downButton;
	};
}

#endif