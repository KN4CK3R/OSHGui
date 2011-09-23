#ifndef __OSHGUI_TABCONTROL_H__
#define __OSHGUI_TABCONTROL_H__

#include "Control.h"
#include <list>

namespace OSHGui
{
	typedef EventHandler<void(Control*)> SelectedIndexChangedEventHandler;

	class TabPage;

	/**
	 * Verwaltet eine Gruppe zusammengehöriger Registerkarten.
	 */
	class TabControl : public Control
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TabControl(Control *parent = 0);
		virtual ~TabControl();

		/**
		 * Ruft die TabPage mit dem entsprechenden Namen ab.
		 *
		 * @param name der Name
		 * @return im Fehlerfall 0
		 */
		TabPage* GetTabPage(const Misc::UnicodeString &name) const;
		/**
		 * Ruft die TabPage am entsprechenden Index ab.
		 *
		 * @param index
		 * @return im Fehlerfall 0
		 */
		TabPage* GetTabPage(int index) const;
		/**
		 * Ruft den SelectedIndexEventHandler für das Steuerelement ab.
		 *
		 * @return selectedIndexEventHandler
		 */
		SelectedIndexChangedEventHandler& GetSelectedIndexChangedEventHandler();
		
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
		virtual bool ContainsPoint(const Drawing::Point &point) const;
		
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
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	private:
		std::list<TabPage*> tabs;
		TabPage *activeTab;

		SelectedIndexChangedEventHandler selectedIndexChangedEventHandler;
	};
}

#endif