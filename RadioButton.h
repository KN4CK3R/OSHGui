#ifndef __OSHGUI_RADIOBUTTON_H__
#define __OSHGUI_RADIOBUTTON_H__

#include "CheckBox.h"

namespace OSHGui
{
	/**
	 * Diese Klasse repr�sentiert einen RadioButton.
	 */
	class RadioButton : public CheckBox
	{	
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		RadioButton(Control *parent = NULL);
		
		/**
		 * Legt den checked-Status fest und passt automatisch
		 * andere RadioButtons in der gleichen Gruppe an.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Legt die Gruppe fest zu der dieser RadioButton geh�rt.
		 *
		 * @param group
		 */
		void SetGroup(int group);
		/**
		 * Gibt die Gruppe zur�ck zu der dieser RadioButton geh�rt.
		 *
		 * @return die Gruppe
		 */
		int GetGroup();
		
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
		virtual void SetCheckedInternal(bool checked);
	
		int group;
	};
}

#endif