#ifndef OSHGUI_LISTBOX_HPP_
#define OSHGUI_LISTBOX_HPP_

#include "Control.hpp"
#include "ScrollBar.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein Steuerlement zum Anzeigen einer Liste von Elementen dar.
	 */
	class OSHGUI_EXPORT ListBox : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		ListBox();
		virtual ~ListBox();
		
		/**
		 * Gibt das Item an der Stelle index zurück.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::AnsiString& GetItem(int index) const;
		/**
		 * Gibt den ausgewählten Index zurück.
		 *
		 * @return der ausgewählte Index
		 */
		int GetSelectedIndex() const;
		/**
		 * Gibt das ausgewählte Item zurück.
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
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		
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
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	private:
		static const Drawing::Size DefaultSize;

		ScrollBar *scrollBar;
		
		int selectedIndex,
			firstVisibleItemIndex;
		bool drag;
		
		Drawing::Rectangle itemsRect;
		
		std::vector<Misc::AnsiString> items;

		SelectedIndexChangedEvent selectedIndexChangedEvent;
		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
	};
}

#endif