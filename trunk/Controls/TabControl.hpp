#ifndef OSHGUI_TABCONTROL_HPP_
#define OSHGUI_TABCONTROL_HPP_

#include <list>
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

				void SetActive(bool active);
				TabPage* GetTabPage() const;

				void Render(Drawing::IRenderer *renderer);

			private:
				static const Drawing::Point DefaultLabelOffset;

				TabPage *tabPage;
				Label *label;

				bool active;
			};

		public:
			TabControlBar();

			virtual void SetForeColor(const Drawing::Color &color);
			virtual void SetBackColor(const Drawing::Color &color);

			void AddTabPage(TabPage *tabPage);
			void RemoveTabPage(TabPage *tabPage);
			TabPage* GetActiveTabPage() const;
			const std::list<TabPage*>& GetTabPages() const;

			void Render(Drawing::IRenderer *renderer);

		private:
			std::list<TabPage*> tabPages;
			TabPage *activeTabPage;
		};
	};
}

#endif