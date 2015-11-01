/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TABCONTROL_HPP
#define OSHGUI_TABCONTROL_HPP

#include <memory>
#include "Button.hpp"

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
	class OSHGUI_EXPORT TabControl : public Control
	{
		friend TabPage;

	public:
		using Control::SetSize;

		/**
		 * Konstruktor der Klasse.
		 */
		TabControl();

		/**
		 * Legt die H�he und Breite des Steuerelements fest.
		 *
		 * \param size
		 */
		virtual void SetSize(const Drawing::SizeI &size) override;
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetForeColor(const Drawing::Color &color) override;
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * \param color
		 */
		virtual void SetBackColor(const Drawing::Color &color) override;
		/**
		 * Ruft die TabPage mit dem entsprechenden Namen ab.
		 *
		 * \param name der Name
		 * \return Zeiger zur TabPage
		 */
		TabPage* GetTabPage(const Misc::AnsiString &name) const;
		/**
		 * Ruft die TabPage am entsprechenden Index ab.
		 *
		 * \param index
		 * \return Zeiger zur TabPage
		 */
		TabPage* GetTabPage(int index) const;
		/**
		 * Legt den Index der derzeit ausgew�hlten Registerkarte fest.
		 *
		 * \param index
		 */
		void SetSelectedIndex(int index);
		/**
		 * Ruft den Index der derzeit ausgew�hlten Registerkarte ab.
		 *
		 * \return index
		 */
		int GetSelectedIndex() const;
		/**
		 * Legt die derzeit ausgew�hlte Registerkarte fest.
		 *
		 * \param tabPage
		 */
		void SetSelectedTabPage(TabPage *tabPage);
		/**
		 * Ruft die derzeit ausgew�hlte Registerkarte ab.
		 *
		 * \return selectedTabPage
		 */
		TabPage* GetSelectedTabPage() const;
		/**
		 * Ruft das SelectedIndexEvent f�r das Steuerelement ab.
		 *
		 * \return selectedIndexEvent
		 */
		SelectedIndexChangedEvent& GetSelectedIndexChangedEvent();
		
		/**
		 * F�gt dem TabControl eine neue TabPage hinzu.
		 *
		 * \param tabPage
		 */
		void AddTabPage(TabPage *tabPage);
		/**
		 * Entfernt eine TabPage aus dem TabControl.
		 *
		 * \param tabPage
		 */
		void RemoveTabPage(TabPage *tabPage);
		
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation() override;
		/**
		 * Veranlasst das Steuerelement, sein Aussehen dem Style anzupassen.
		 *
		 * \param style
		 */
		virtual void ApplyStyle(const Drawing::Style &style) override;

	protected:
		virtual void DrawSelf(Drawing::RenderContext &context) override;

	private:
		static const Drawing::SizeI DefaultSize;
		static const int ButtonPageDistance = 4;

		void CalculateButtonLocationAndCount();

		struct TabPageButtonBinding;
		void SelectBinding(TabPageButtonBinding &binding);

		SelectedIndexChangedEvent selectedIndexChangedEvent_;

		class TabControlButton;
		struct TabPageButtonBinding
		{
			int Index;
			TabPage *TabPage;
			TabControlButton *Button;
		};

		class TabControlButton : public Button
		{
		public:
			TabControlButton(TabPageButtonBinding &binding);

			void SetActive(bool active);

		protected:
			virtual void OnMouseClick(const MouseMessage &mouse) override;

		private:
			TabPageButtonBinding &binding;

			bool active;
		};

		TabPageButtonBinding *selected;
		std::vector<std::unique_ptr<TabPageButtonBinding>> bindings;

		class TabControlSwitchButton : public Control
		{
		public:
			enum class TabControlSwitchButtonDirection
			{
				Left,
				Right
			};

			static const Drawing::SizeI DefaultSize;

			TabControlSwitchButton(TabControlSwitchButtonDirection direction);

		protected:
			virtual void PopulateGeometry() override;

		private:
			TabControlSwitchButtonDirection direction_;
		};

		int startIndex;
		int maxIndex;
		TabControlSwitchButton *lastSwitchButton;
		TabControlSwitchButton *nextSwitchButton;
	};
}

#endif