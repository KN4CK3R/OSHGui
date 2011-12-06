#ifndef OSHGUI_TABCONTROL_HPP_
#define OSHGUI_TABCONTROL_HPP_

#include "ContainerControl.hpp"

namespace OSHGui
{
	class Label;
	class TabPage;

	/**
	 * Verwaltet eine Gruppe zusammengehöriger Registerkarten.
	 */
	class OSHGUI_EXPORT TabControl : public Control
	{
		friend TabPage;
		class TabControlBar;

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
		virtual bool Intersect(const Drawing::Point &point) const;

		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	private:
		static const Drawing::Size DefaultSize;

		TabControlBar *tabControlBar;

		SelectedIndexChangedEvent selectedIndexChangedEvent;

		class TabControlBar : public ContainerControl
		{
			class TabControlBarButton : public Control
			{
			public:
				TabControlBarButton(TabPage *tabPage);
				~TabControlBarButton();

				virtual void SetForeColor(const Drawing::Color &color);
				virtual void SetBackColor(const Drawing::Color &color);
				virtual void SetText(const Misc::AnsiString &text);
				void SetActive(bool active);
				TabPage* GetTabPage() const;

				virtual bool Intersect(const Drawing::Point &point) const;

				void Render(Drawing::IRenderer *renderer);

			private:
				static const Drawing::Point DefaultLabelOffset;

				TabPage *tabPage;
				Label *label;

				bool active;
			};

		public:
			struct TabPageButtonBinding
			{
				int index;
				TabPage *tabPage;
				TabControlBarButton *button;
			};

			TabControlBar();

			virtual void SetForeColor(const Drawing::Color &color);
			virtual void SetBackColor(const Drawing::Color &color);

			void AddTabPage(TabPage *tabPage);
			void RemoveTabPage(TabPage *tabPage);
			void SetSelectedIndex(int index);
			void SetSelectedTabPage(TabPage *tabPage);
			TabPage* GetSelectedTabPage() const;
			const std::list<TabPageButtonBinding>& GetTabPageButtonBindings() const;

			void Render(Drawing::IRenderer *renderer);

		private:
			std::list<TabPageButtonBinding> bindings;
	
			TabPage *selectedTabPage;
		};
	};
}

#endif