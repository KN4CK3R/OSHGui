#ifndef OSHGUI_LISTBOX_HPP_
#define OSHGUI_LISTBOX_HPP_

#include "ContainerControl.hpp"
#include "ScrollBar.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Steuerlement zum Anzeigen einer Liste von Elementen dar.
	 */
	class OSHGUI_EXPORT ListBox : public ContainerControl
	{
	public:
		using ContainerControl::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ListBox();
		virtual ~ListBox();
		
		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
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
		 * Gibt das Item an der Stelle index zurück.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::AnsiString& GetItem(int index) const;
		/**
		 * Legt den ausgewählten Index fest.
		 *
		 * @param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Gibt den ausgewählten Index zurück.
		 *
		 * @return der ausgewählte Index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt das ausgewählte Item fest.
		 *
		 * @param item
		 */
		void SetSelectedItem(const Misc::AnsiString &item);
		/**
		 * Ruft das ausgewählte Item ab.
		 *
		 * @return das Item
		 */
		const Misc::AnsiString& GetSelectedItem() const;
		/**
		 * Gibt die Anzahl der Items zurück.
		 *
		 * @return Anzahl der Items
		 */
		int GetItemsCount() const;
		/**
		 * Ruft das SelectedIndexEvent für das Steuerelement ab.
		 *
		 * @return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();

		/**
		 * Fügt ein neues Item hinzu.
		 *
		 * @param text der Text des Items
		 */
		void AddItem(const Misc::AnsiString &text);
		/**
		 * Fügt ein neues Item am gewählten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des Items
		 */
		void InsertItem(int index, const Misc::AnsiString &text);
		/**
		 * Löscht das Item am gewählten Index.
		 *
		 * @param index
		 */
		void RemoveItem(int index);
		/**
		 * Löscht alle Items.
		 */
		void Clear();
		/**
		 * Selektiert ein Item.
		 *
		 * @param index
		 */
		void SelectItem(int index);

		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual bool OnKeyPress(const KeyboardMessage &keyboard);

	private:
		static const Drawing::Size DefaultSize;
		
		int selectedIndex;
		int firstVisibleItemIndex;
		int maxVisibleItems;
		
		Drawing::Rectangle itemsRect;
		Drawing::Size itemAreaSize;
		
		std::vector<Misc::AnsiString> items;

		SelectedIndexChangedEvent selectedIndexChangedEvent;

		ScrollBar *scrollBar;
	};
}

#endif