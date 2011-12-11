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
		 * Gibt das Item an der Stelle index zur�ck.
		 *
		 * @param index
		 * @return das Item
		 */
		const Misc::AnsiString& GetItem(int index) const;
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
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		
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
		 * Selektiert ein Item.
		 *
		 * @param index
		 */
		void SelectItem(int index);

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
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