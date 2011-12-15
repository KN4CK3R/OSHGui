#ifndef OSHGUI_TABCONTROL_HPP
#define OSHGUI_TABCONTROL_HPP

#include "ContainerControl.hpp"

namespace OSHGui
{
	class Label;
	class TabPage;

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
		virtual void SetSize(const Drawing::Size &size);
		/**
		 * Legt die Fordergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetForeColor(const Drawing::Color &color);
		/**
		 * Legt die Hintergrundfarbe des Steuerelements fest.
		 *
		 * @param color
		 */
		virtual void SetBackColor(const Drawing::Color &color);
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
		virtual bool Intersect(const Drawing::Point &point) const;
		/**
		 * Berechnet die absolute Position des Steuerelements.
		 */
		virtual void CalculateAbsoluteLocation();

		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

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

			virtual void SetForeColor(const Drawing::Color &color);
			virtual void SetBackColor(const Drawing::Color &color);
			virtual void SetText(const Misc::AnsiString &text);
			void SetActive(bool active);

			virtual bool Intersect(const Drawing::Point &point) const;
			virtual void CalculateAbsoluteLocation();

			void Render(Drawing::IRenderer *renderer);

		protected:
			virtual void OnMouseClick(const MouseMessage &mouse);

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

			virtual bool Intersect(const Drawing::Point &point) const;

			void Render(Drawing::IRenderer *renderer);

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