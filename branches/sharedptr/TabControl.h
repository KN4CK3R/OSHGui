#ifndef __OSHGUI_TABCONTROL_H__
#define __OSHGUI_TABCONTROL_H__

#include <list>

#include "Control.h"

namespace OSHGui
{
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
		TabControl(const std::shared_ptr<Control> &parent = 0);
		virtual ~TabControl();

		std::shared_ptr<TabPage> GetTabPage(const Misc::UnicodeString &name);
		std::shared_ptr<TabPage> GetTabPage(int index);
		void AddTabPage(const std::shared_ptr<TabPage> &tabPage);
		
		void RemoveTabPage(const std::shared_ptr<TabPage> &tabPage);
		
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool ContainsPoint(const Drawing::Point &point);
		
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
		virtual Event::NextEventTypes ProcessEvent(const std::shared_ptr<Event> &event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(const std::shared_ptr<Drawing::IRenderer> &renderer);

	private:
		std::list<std::shared_ptr<TabPage>> tabs;
		std::shared_ptr<TabPage> activeTab;
	};
}

#endif