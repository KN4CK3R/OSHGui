#ifndef __OSHGUI_TABPAGE_H__
#define __OSHGUI_TABPAGE_H__

#include "Panel.h"

namespace OSHGui
{
	class TabControl;

	/**
	 * Wird zum Gruppieren von Auflistungen von Steuerelementen verwendet.
	 */
	class TabPage : public Panel
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		TabPage(TabControl *parent = 0);
		
		/**
		 * Legt den Text fest.
		 *
		 * @param text
		 */
		void SetText(const Misc::UnicodeString &text);
		/**
		 * Ruft den Text ab.
		 *
		 * @return der Text
		 */
		const Misc::UnicodeString& GetText();
		
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

	protected:
		
		Misc::UnicodeString text;
	};
}

#endif