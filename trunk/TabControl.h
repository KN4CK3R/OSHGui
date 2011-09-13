#ifndef __OSHGUI_TABCONTROL_H__
#define __OSHGUI_TABCONTROL_H__

#include "Control.h"
#include <list>

namespace OSHGui
{
	class TabPage;

	/**
	 * Verwaltet eine Gruppe zusammengeh�riger Registerkarten.
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

		TabPage* GetTabPage(const Misc::UnicodeString &name);
		TabPage* GetTabPage(int index);
		void AddTabPage(TabPage *tabPage);
		
		void RemoveTabPage(TabPage *tabPage);
		
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
		/**
		 * Veranlasst das Steuerelemt seine interne Struktur neu zu berechnen.
		 * Wird au�erdem f�r alle Kindelemente aufgerufen.
		 *
		 * Sollte nicht vom Benutzer aufgerufen werden!
		 */
		virtual void Invalidate();

		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual Event::NextEventTypes ProcessEvent(Event *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);

	private:
		std::list<TabPage*> tabs;
		TabPage *activeTab;
	};
}

#endif