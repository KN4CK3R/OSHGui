/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_LISTBOX_HPP
#define OSHGUI_LISTBOX_HPP

#include "Control.hpp"

namespace OSHGui
{
	class ScrollBar;

	/**
	 * Tritt ein, wenn sich der Wert der SelectedIndex-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> SelectedIndexChangedEvent;
	typedef EventHandler<void(Control*)> SelectedIndexChangedEventHandler;

	/**
	 * Stellt ein Steuerlement zum Anzeigen einer Liste von Elementen dar.
	 */
	class OSHGUI_EXPORT ListBox : public Control
	{
	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		ListBox();
		virtual ~ListBox();
		
		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
		/**
		 * Legt die Schriftart des Texts im Steuerelement fest.
		 *
		 * \param font
		 */
		virtual void SetFont(const Drawing::FontPtr &font) override;
		/**
		 * Legt fest, ob die ListBox automatisch zum Element scrollen soll, wenn es hinzugef�gt wird.
		 *
		 * \param autoScrollEnabled
		 */
		void SetAutoScrollEnabled(bool autoScrollEnabled);
		/**
		 * Ruft ab, ob die ListBox automatisch zum Element scrollen soll, wenn es hinzugef�gt wird.
		 *
		 * \return autoScrollEnabled
		 */
		bool GetAutoScrollEnabled() const;
		/**
		 * Gibt das Item an der Stelle index zur�ck.
		 *
		 * \param index
		 * \return das Item
		 */
		const Misc::AnsiString& GetItem(int index) const;
		/**
		 * Legt den ausgew�hlten Index fest.
		 *
		 * \param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Gibt den ausgew�hlten Index zur�ck.
		 *
		 * \return der ausgew�hlte Index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt das ausgew�hlte Item fest.
		 *
		 * \param item
		 */
		void SetSelectedItem(const Misc::AnsiString &item);
		/**
		 * Ruft das ausgew�hlte Item ab.
		 *
		 * \return das Item
		 */
		const Misc::AnsiString& GetSelectedItem() const;
		/**
		 * Gibt die Anzahl der Items zur�ck.
		 *
		 * \return Anzahl der Items
		 */
		int GetItemsCount() const;
		/**
		 * Ruft das SelectedIndexEvent f�r das Steuerelement ab.
		 *
		 * \return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();
		/**
		 * Erweitert die Gr��e damit die angegebene Anzahl an Items sichtbar ist.
		 */
		void ExpandSizeToShowItems(int count);

		/**
		 * F�gt ein neues Item hinzu.
		 *
		 * \param text der Text des Items
		 */
		void AddItem(const Misc::AnsiString &text);
		/**
		 * F�gt ein neues Item am gew�hlten Index hinzu.
		 *
		 * \param index
		 * \param text der Text des Items
		 */
		void InsertItem(int index, const Misc::AnsiString &text);
		/**
		 * L�scht das Item am gew�hlten Index.
		 *
		 * \param index
		 */
		void RemoveItem(int index);
		/**
		 * L�scht alle Items.
		 */
		void Clear();

		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * \param point
		 * \return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;

		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Theme anzupassen.
		 *
		 * \param theme
		 */
		virtual void ApplyTheme(const Drawing::Theme &theme) override;

		virtual void DrawSelf(Drawing::RenderContext &context) override;
	
	protected:
		virtual void PopulateGeometry() override;

		virtual void OnMouseClick(const MouseMessage &mouse) override;
		virtual void OnMouseScroll(const MouseMessage &mouse) override;
		virtual bool OnKeyDown(const KeyboardMessage &keyboard) override;
		virtual bool OnKeyPress(const KeyboardMessage &keyboard) override;

	private:
		static const Drawing::SizeF DefaultSize;
		static const Drawing::SizeF DefaultItemAreaPadding;
		static const int DefaultItemPadding;

		void CheckForScrollBar();
		
		int selectedIndex;
		int firstVisibleItemIndex;
		int maxVisibleItems;
		bool autoScrollEnabled;
		
		Drawing::RectangleF itemsRect;
		Drawing::SizeF itemAreaSize;
		
		std::vector<Misc::AnsiString> items;

		SelectedIndexChangedEvent selectedIndexChangedEvent;

		ScrollBar *scrollBar;
	};
}

#endif