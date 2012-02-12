/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_COMBOBOX_HPP
#define OSHGUI_COMBOBOX_HPP

#include "ContainerControl.hpp"
#include "Button.hpp"

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
		 * Ruft ab, ob das Steuerelement den Fokus besitzt.
		 *
		 * @return isFocused
		 */
		virtual bool GetIsFocused() const;
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
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Setzt den Eingabefokus auf das Steuerelement.
		 */
		virtual void Focus();

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	private:
		static const Drawing::Size DefaultSize;
		static const int MaxListBoxHeight = 80;

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