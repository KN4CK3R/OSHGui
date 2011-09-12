#ifndef __OSHGUI_LISTBOX_H__
#define __OSHGUI_LISTBOX_H__

#include <vector>
#include "Control.h"
#include "Panel.h"
#include "ScrollBar.h"
#include "ListItem.h"

namespace OSHGui
{
	/**
	 * Stellt ein Steuerlement zum Anzeigen einer Liste von Elementen dar.
	 */
	class ListBox : public Control
	{	
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		ListBox(const std::shared_ptr<Control> &parent = 0);
		virtual ~ListBox();
		
		/**
		 * Gibt das ListItem an der Stelle index zurück.
		 *
		 * @param index
		 * @return 0, falls der Index nicht existiert
		 */
		std::shared_ptr<ListItem> GetItem(int index);
		/**
		 * Gibt den ausgewählten Index zurück.
		 *
		 * @return der ausgewählte Index
		 */
		int GetSelectedIndex();
		/**
		 * Gibt das ausgewählte ListItem zurück.
		 *
		 * @return 0, falls kein ListItem ausgewählt wurde
		 */
		std::shared_ptr<ListItem> GetSelectedItem();
		/**
		 * Gibt die Anzahl der ListItems zurück.
		 *
		 * @return Anzahl der ListItems
		 */
		int GetItemsCount();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus();
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Fügt ein neues ListItem hinzu.
		 *
		 * @param text der Text des ListItems
		 * @return im Fehlerfall false
		 */
		bool AddItem(const Misc::UnicodeString &text);
		/**
		 * Fügt ein neues ListItem am gewählten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des ListItems
		 * @return im Fehlerfall false
		 */
		bool InsertItem(int index, const Misc::UnicodeString &text);
		/**
		 * Löscht das ListItem am gewählten Index.
		 *
		 * @param index
		 * @return im Fehlerfall false
		 */
		bool RemoveItem(int index);
		/**
		 * Löscht alle ListItems.
		 */
		bool Clear();
		/**
		 * Selektiert ein ListItem.
		 *
		 * @param index
		 */
		void SelectItem(int index);
		
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
		virtual Event::NextEventTypes ProcessEvent(const std::shared_ptr<Event> &event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(const std::shared_ptr<Drawing::IRenderer> &renderer);
	
	protected:
		ScrollBar scrollBar;
		
		int selectedIndex,
			firstVisibleItemIndex;
		bool drag;
		
		Drawing::Rectangle itemsRect;
		
		std::vector<std::shared_ptr<ListItem>> items;
	};
}

#endif