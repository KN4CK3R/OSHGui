/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TABCONTROL_HPP
#define OSHGUI_TABCONTROL_HPP

#include "ContainerControl.hpp"

namespace OSHGui
{
	class Label;
	class TabPage;

	/**
	 * Verwaltet eine Gruppe zusammengehöriger Registerkarten.
	 */
	class OSHGUI_EXPORT TabControl : public ContainerControl
	{
		friend TabPage;

	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TabControl();
		virtual ~TabControl();

		/**
		 * Legt die Höhe und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::Size &size) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(Drawing::Color color) override;
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetBackColor(Drawing::Color color) override;
		/**
		 * Ruft die TabPage mit dem entsprechenden Namen ab.
		 *
		 * @param name der Name
		 * @return Zeiger zur TabPage
		 */
		TabPage* GetTabPage(const Misc::AnsiString &name) const;
		/**
		 * Ruft die TabPage am entsprechenden Index ab.
		 *
		 * @param index
		 * @return Zeiger zur TabPage
		 */
		TabPage* GetTabPage(int index) const;
		/**
		 * Legt den Index der derzeit ausgewählten Registerkarte fest.
		 *
		 * @param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Ruft den Index der derzeit ausgewählten Registerkarte ab.
		 *
		 * @return index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt die derzeit ausgewählte Registerkarte fest.
		 *
		 * @param tabPage
		 */
		void SetSelectedTabPage(TabPage *tabPage);
		/**
		 * Ruft die derzeit ausgewählte Registerkarte ab.
		 *
		 * @return selectedTabPage
		 */
		TabPage* GetSelectedTabPage() const;
		/**
		 * Ruft das SelectedIndexEvent für das Steuerelement ab.
		 *
		 * @return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();
		
		/**
		 * Fügt dem TabControl eine neue TabPage hinzu.
		 *
		 * @param tabPage
		 */
		void AddTabPage(TabPage *tabPage);
		/**
		 * Entfernt eine TabPage aus dem TabControl.
		 *
		 * @param tabPage
		 */
		void RemoveTabPage(TabPage *tabPage);
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer) override;

	private:
		static const Drawing::Size DefaultSize;

		void CalculateButtonLocationAndCount();

		SelectedIndexChangedEvent selectedIndexChangedEvent;

		class TabControlButton;
		struct TabPageButtonBinding
		{
			int index;
			TabPage *tabPage;
			TabControlButton *button;
		};

		class TabControlButton : public Control
		{
		public:
			TabControlButton(TabPageButtonBinding *binding);
			~TabControlButton();

			virtual void SetForeColor(Drawing::Color color) override;
			void SetText(const Misc::AnsiString &text);
			void SetActive(bool active);

			virtual bool Intersect(const Drawing::Point &point) const override;
			virtual void CalculateAbsoluteLocation() override;

			virtual void Render(Drawing::IRenderer *renderer) override;

		protected:
			virtual void OnMouseClick(const MouseMessage &mouse) override;

		private:
			static const Drawing::Point DefaultLabelOffset;

			TabPageButtonBinding *binding;
			Label *label;

			bool active;
		};

		TabPageButtonBinding *selected;
		std::vector<TabPageButtonBinding*> bindings;

		class TabControlSwitchButton : public Control
		{
		public:
			static const Drawing::Size DefaultSize;

			TabControlSwitchButton(int direction);

			virtual bool Intersect(const Drawing::Point &point) const override;

			virtual void Render(Drawing::IRenderer *renderer) override;

		private:
			int direction;
		};

		int startIndex;
		int maxIndex;
		TabControlSwitchButton *lastSwitchButton;
		TabControlSwitchButton *nextSwitchButton;
	};
}

#endif