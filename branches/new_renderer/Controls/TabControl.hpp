/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
	 * Tritt ein, wenn sich der Wert der SelectedIndex-Eigenschaft �ndert.
	 */
	typedef Event<void(Control*)> SelectedIndexChangedEvent;
	typedef EventHandler<void(Control*)> SelectedIndexChangedEventHandler;

	/**
	 * Verwaltet eine Gruppe zusammengeh�riger Registerkarten.
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
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * @param size
		 */
		virtual void SetSize(const Drawing::SizeF &size) override;
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
		 * Legt den Index der derzeit ausgew�hlten Registerkarte fest.
		 *
		 * @param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Ruft den Index der derzeit ausgew�hlten Registerkarte ab.
		 *
		 * @return index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt die derzeit ausgew�hlte Registerkarte fest.
		 *
		 * @param tabPage
		 */
		void SetSelectedTabPage(TabPage *tabPage);
		/**
		 * Ruft die derzeit ausgew�hlte Registerkarte ab.
		 *
		 * @return selectedTabPage
		 */
		TabPage* GetSelectedTabPage() const;
		/**
		 * Ruft das SelectedIndexEvent f�r das Steuerelement ab.
		 *
		 * @return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();
		
		/**
		 * F�gt dem TabControl eine neue TabPage hinzu.
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
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::PointF &point) const override;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;
		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Theme anzupassen.
		 *
		 * @param theme
		 */
		virtual void ApplyTheme(const Drawing::Theme &theme) override;

	protected:
		virtual void DrawSelf(Drawing::RenderContext &context) override;

	private:
		static const Drawing::SizeF DefaultSize;

		void CalculateButtonLocationAndCount();

		struct TabPageButtonBinding;
		void SelectBinding(TabPageButtonBinding &binding);

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
			TabControlButton(TabPageButtonBinding &binding);

			virtual void SetForeColor(Drawing::Color color) override;
			void SetText(const Misc::AnsiString &text);
			void SetActive(bool active);

			virtual bool Intersect(const Drawing::PointF &point) const override;
			virtual void CalculateAbsoluteLocation() override;

		protected:
			virtual void DrawSelf(Drawing::RenderContext &context) override;
			virtual void PopulateGeometry() override;

			virtual void OnMouseClick(const MouseMessage &mouse) override;

		private:
			static const Drawing::PointF DefaultLabelOffset;

			TabPageButtonBinding &binding;
			std::unique_ptr<Label> label;

			bool active;
		};

		TabPageButtonBinding *selected;
		std::vector<TabPageButtonBinding*> bindings;

		class TabControlSwitchButton : public Control
		{
		public:
			static const Drawing::SizeF DefaultSize;

			TabControlSwitchButton(int direction);

			virtual bool Intersect(const Drawing::PointF &point) const override;

		protected:
			virtual void PopulateGeometry() override;

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