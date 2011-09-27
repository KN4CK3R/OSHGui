#ifndef __OSHGUI_LISTBOX_H__
#define __OSHGUI_LISTBOX_H__

#include <vector>
#include "Event\EventHandler.h"
#include "Control.h"
#include "Panel.h"
#include "ScrollBar.h"

namespace OSHGui
{
	/**
	 * Tritt auf, wenn sich der Wert der SelectedIndex-Eigenschaft �ndert.
	 */
	typedef EventHandler<void(Control*)> SelectedIndexChangedEvent;

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
		ListBox(Control *parent = 0);
		virtual ~ListBox();
		
		/**
		 * Gibt das Item an der Stelle index zur�ck.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::UnicodeString& GetItem(int index) const;
		/**
		 * Gibt den ausgew�hlten Index zur�ck.
		 *
		 * @return der ausgew�hlte Index
		 */
		int GetSelectedIndex() const;
		/**
		 * Gibt das ausgew�hlte Item zur�ck.
		 *
		 * @return das Item
		 */
		const Misc::UnicodeString& GetSelectedItem() const;
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
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
		/**
		 * F�gt ein neues Item hinzu.
		 *
		 * @param text der Text des Items
		 * @return im Fehlerfall false
		 */
		bool AddItem(const Misc::UnicodeString &text);
		/**
		 * F�gt ein neues Item am gew�hlten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des Items
		 * @return im Fehlerfall false
		 */
		bool InsertItem(int index, const Misc::UnicodeString &text);
		/**
		 * L�scht das Item am gew�hlten Index.
		 *
		 * @param index
		 * @return im Fehlerfall false
		 */
		bool RemoveItem(int index);
		/**
		 * L�scht alle Items.
		 */
		bool Clear();
		/**
		 * Selektiert ein Item.
		 *
		 * @param index
		 */
		void SelectItem(int index);
		
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
		ScrollBar scrollBar;
		
		int selectedIndex,
			firstVisibleItemIndex;
		bool drag;
		
		Drawing::Rectangle itemsRect;
		
		std::vector<Misc::UnicodeString> items;

		SelectedIndexChangedEvent selectedIndexChangedEvent;
	};
}

#endif