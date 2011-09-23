#ifndef __OSHGUI_CHECKBOX_H__
#define __OSHGUI_CHECKBOX_H__

#include "Event\EventHandler.h"
#include "Label.h"

namespace OSHGui
{
	typedef EventHandler<void(Control*)> CheckedChangedEvent;

	/**
	 * Stellt ein CheckBox-Steuerelement dar.
	 */
	class CheckBox : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		CheckBox(Control *parent = 0);
		
		/**
		 * Legt den checked-Status fest.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zur�ck.
		 *
		 * @return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Ruft das CheckedChangeEvent f�r das Steuerelement ab.
		 *
		 * @return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * �berpr�ft, ob das Steuerelement den Fokus �bernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;

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
	
	protected:
		bool checked;
		Drawing::Point checkBoxPosition,
					   textPosition;
					   
		CheckedChangedEvent checkedChangedEvent;
	};
}

#endif