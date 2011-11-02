#ifndef OSHGUI_LISTBOX_HPP_
#define OSHGUI_LISTBOX_HPP_

#include <vector>
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
		 *
		 * @param parent das Elternsteuerelement
		 */
		ListBox(Control *parent);
		virtual ~ListBox();
		
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
		 * Ruft das KeyDownEvent für das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent für das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent für das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;
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
		 */
		void AddItem(const Misc::UnicodeString &text);
		/**
		 * Fügt ein neues Item am gewählten Index hinzu.
		 *
		 * @param index
		 * @param text der Text des Items
		 */
		void InsertItem(int index, const Misc::UnicodeString &text);
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
		virtual IEvent::NextEventTypes ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
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
		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
	};
}

#endif