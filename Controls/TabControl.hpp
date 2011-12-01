#ifndef OSHGUI_TABCONTROL_HPP_
#define OSHGUI_TABCONTROL_HPP_

#include <list>
#include "Control.hpp"

namespace OSHGui
{
	class TabPage;

	/**
	 * Verwaltet eine Gruppe zusammengehöriger Registerkarten.
	 */
	class OSHGUI_EXPORT TabControl : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		TabControl();
		virtual ~TabControl();

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
		virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	private:
		std::list<TabPage*> tabs;
		TabPage *activeTab;

		SelectedIndexChangedEvent selectedIndexChangedEvent;
	};
}

#endif