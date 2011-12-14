#ifndef OSHGUI_COMBOBOX_HPP_
#define OSHGUI_COMBOBOX_HPP_

#include "ContainerControl.hpp"
#include "Button.hpp"

#define COMBOBOX_ITEM_HEIGHT 22
#define COMBOBOX_MAX_HEIGHT 220

namespace OSHGui
{
	class ListBox;
	class ScrollBar;

	/**
	 * Stellt ein Kombinationsfeld-Steuerelement dar.
	 */
	class OSHGUI_EXPORT ComboBox : public ContainerControl
	{
	public:
		using ContainerControl::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ComboBox();
		virtual ~ComboBox();
		
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * @param font
		 */
		virtual void SetFont(const std::shared_ptr<Drawing::IFont> &font);
		/**
		 * Legt die Hintergrundfarbe des DropDown-Steuerelements fest.
		 *
		 * @param color
		 */
		void SetDropDownColor(const Drawing::Color &color);
		/**
		 * Ruft die Hintergrundfarbe des DropDown-Steuerelements ab.
		 *
		 * @return color
		 */
		Drawing::Color GetDropDownColor() const;
		/**
		 * Gibt das Item an der Stelle index zur�ck.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::AnsiString& GetItem(int index) const;
		/**
		 * Legt den ausgew�hlten Index fest.
		 *
		 * @param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Gibt den ausgew�hlten Index zur�ck.
		 *
		 * @return der ausgew�hlte Index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt das ausgew�hlte Item fest.
		 *
		 * @param item
		 */
		void SetSelectedItem(const Misc::AnsiString &item);
		/**
		 * Ruft das ausgew�hlte Item ab.
		 *
		 * @return das Item
		 */
		const Misc::AnsiString& GetSelectedItem() const;
		/**
		 * Gibt die Anzahl der Items zur�ck.
		 *
		 * @return Anzahl der Items
		 */
		int GetItemsCount() const;
		/**
		 * Ruft das SelectedIndexEvent f�r das Steuerelement ab.
		 *
		 * @return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();
		
		/**
		 * F�gt ein neues Item hinzu.
		 *
		 * @param text der Text des Items
		 */
		void AddItem(const Misc::AnsiString &text);
		/**
		 * F�gt ein neues Item am gew�hlten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des Items
		 */
		void InsertItem(int index, const Misc::AnsiString &text);
		/**
		 * L�scht das Item am gew�hlten Index.
		 *
		 * @param index
		 */
		void RemoveItem(int index);
		/**
		 * L�scht alle Items.
		 */
		void Clear();
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void OnLostFocus();

	private:
		static const Drawing::Size DefaultSize;

		void Expand();
		void Collapse();
		
		bool droppedDown;
		
		Drawing::Color dropDownColor;
		
		class ComboBoxButton : public Button
		{
		public:
			using Button::SetSize;

			virtual void SetSize(const Drawing::Size &size);

			virtual bool Intersect(const Drawing::Point &point) const;

			virtual void Render(Drawing::IRenderer *renderer);

		protected:
			virtual bool OnKeyDown(const KeyboardMessage &keyboard);

		private:
			Drawing::Size realSize;
			Drawing::Point arrowAbsoluteLocation;
		};
		ComboBoxButton *button;

		ListBox *listBox;
	};
}

#endif