#ifndef __OSHGUI_COMBOBOX_H__
#define __OSHGUI_COMBOBOX_H__

#include "Button.h"
#include "ScrollBar.h"
#include "ListItem.h"

#define COMBOBOX_ITEM_HEIGHT 22
#define COMBOBOX_MAX_HEIGHT 220

namespace OSHGui
{
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
		ComboBox(Control *parent = NULL);
		virtual ~ComboBox();
		
		/**
		 * Gibt das ListItem an der Stelle index zur�ck.
		 *
		 * @param index
		 * @return NULL, falls der Index nicht existiert
		 */
		ListItem* GetItem(int index);
		/**
		 * Gibt den ausgew�hlten Index zur�ck.
		 *
		 * @return der ausgew�hlte Index
		 */
		int GetSelectedIndex();
		/**
		 * Gibt das ausgew�hlte ListItem zur�ck.
		 *
		 * @return NULL, falls kein ListItem ausgew�hlt wurde
		 */
		ListItem* GetSelectedItem();
		/**
		 * Gibt die Anzahl der ListItems zur�ck.
		 *
		 * @return Anzahl der ListItems
		 */
		int GetItemsCount();
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * F�gt ein neues ListItem hinzu.
		 *
		 * @param text der Text des ListItems
		 * @return im Fehlerfall false
		 */
		bool AddItem(const Misc::UnicodeString &text);
		/**
		 * F�gt ein neues ListItem am gew�hlten Index hinzu.
		 *
		 * @param text der Text des ListItems
		 * @return im Fehlerfall false
		 */
		bool InsertItem(int index, const Misc::UnicodeString &text);
		/**
		 * L�scht das ListItem am gew�hlten Index.
		 *
		 * @param index
		 * @return im Fehlerfall false
		 */
		bool RemoveItem(int index);
		/**
		 * L�scht alle ListItems.
		 */
		bool Clear();
		/**
		 * Selektiert ein ListItem.
		 *
		 * @param index
		 */
		void SelectItem(int newIndex);

		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();
		
		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
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
		
		std::vector<ListItem*> items;
	};
}

#endif