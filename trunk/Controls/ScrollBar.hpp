#ifndef OSHGUI_SCROLLBAR_HPP_
#define OSHGUI_SCROLLBAR_HPP_

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
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(const Drawing::Color &color);
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
		 * Legt die Größe des sichtbaren Ausschnitts fest.
		 *
		 * @param pageSize
		 */
		void SetPageSize(int pageSize);
		/**
		 * Gibt die Größe des sichtbaren Ausschnitts zurück.
		 *
		 * @return die Größe des sichtbaren Ausschnitts
		 */
		int GetPageSize();
		
		/**
		 * Verschiebt den ScrollButton, damit er den Index anzeigt.
		 *
		 * @return false, falls die ScrollBar nicht aktiv ist
		 */
		bool ShowItem(int index);
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();
		
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void OnMouseDown(const MouseMessage &mouse);
		virtual void OnMouseUp(const MouseMessage &mouse);
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual void OnMouseMove(const MouseMessage &mouse);

	private:
		static const int MinimumSliderHeight = 25;
		static const int DefaultScrollBarWidth = 14;

		void SetValueInternal(int value);
		void Scroll(int delta);
		void UpdateSliderRect();
		void Capture();
		
		int position,
			range,
			pageSize;
		bool drag;
		
		Drawing::Rectangle upButtonRect,
						   downButtonRect,
						   trackRect,
						   sliderRect;
		///////////////////////////
		int value;
		int pixelsPerTick;
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
			
			virtual void SetSize(const Drawing::Size &size);

			virtual bool Intersect(const Drawing::Point &point) const;
			virtual void CalculateAbsoluteLocation();

			virtual void Render(Drawing::IRenderer *renderer);

		private:
			int direction;

			Drawing::Point iconLocation;
		};
		ScrollBarButton *upButton;
		ScrollBarButton *downButton;

		/*class ScrollBarSliderTrack : public Control
		{
		public:
			using Control::SetSize;

			ScrollBarSliderTrack();

			virtual bool Intersect(const Drawing::Point &point) const;

		protected:
			virtual void OnMouseDown(const MouseMessage &mouse);
			virtual void OnMouseUp(const MouseMessage &mouse);
			virtual void OnMouseClick(const MouseMessage &mouse);
			virtual void OnMouseMove(const MouseMessage &mouse);

		private:
			Drawing::Point sliderLocation;
			Drawing::Point sliderAbsoluteLocation;
			Drawing::Size sliderSize;
			int pixelsPerTick;
			bool drag;
		};*/
	};
}

#endif