#ifndef __OSHGUI_COMBOBOX_H__
#define __OSHGUI_COMBOBOX_H__

#include "Event\EventHandler.h"
#include "Button.h"
#include "ScrollBar.h"

#define COMBOBOX_ITEM_HEIGHT 22
#define COMBOBOX_MAX_HEIGHT 220

namespace OSHGui
{
	/**
	 * Tritt auf, wenn sich der Wert der SelectedIndex-Eigenschaft ändert.
	 */
	typedef EventHandler<void(Control*)> SelectedIndexChangedEvent;

	/**
	 * Stellt ein Kombinationsfeld-Steuerelement dar.
	 */
	class ComboBox : public Button
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ComboBox(Control *parent = 0);
		virtual ~ComboBox();
		
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
		 * Gibt das Item an der Stelle index zurück.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::UnicodeString& GetItem(int index) const;
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
		const Misc::UnicodeString& GetSelectedItem() const;
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
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * Fügt ein neues Item hinzu.
		 *
		 * @param text der Text des Items
		 * @return im Fehlerfall false
		 */
		bool AddItem(const Misc::UnicodeString &text);
		/**
		 * Fügt ein neues Item am gewählten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des Items
		 * @return im Fehlerfall false
		 */
		bool InsertItem(int index, const Misc::UnicodeString &text);
		/**
		 * Löscht das Item am gewählten Index.
		 *
		 * @param index
		 * @return im Fehlerfall false
		 */
		bool RemoveItem(int index);
		/**
		 * Löscht alle Items.
		 */
		bool Clear();
		/**
		 * Selektiert ein Item.
		 *
		 * @param newIndex
		 */
		void SelectItem(int newIndex);

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
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void SetFocus(bool focus);

		ScrollBar scrollBar;
		
		int dropdownHeight;
		
		int selectedIndex,
			firstVisibleItemIndex,
			mouseOverItemIndex;
		bool open;
		
		Drawing::Rectangle dropDownRect,
						   itemsRect;
		Drawing::Color dropDownColor;
		
		std::vector<Misc::UnicodeString> items;

		SelectedIndexChangedEvent selectedIndexChangedEvent;
	};
}

#endif