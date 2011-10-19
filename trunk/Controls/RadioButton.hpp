#ifndef OSHGUI_RADIOBUTTON_H_
#define OSHGUI_RADIOBUTTON_H_

#include "CheckBox.hpp"

namespace OSHGui
{
	/**
	 * Erm�glicht Benutzern bei Verwendung weiterer RadioButton-Steuerelemete
	 * die Auswahl einer Option aus mehreren Auswahlm�glichkeiten.
	 */
	class OSHGUI_EXPORT RadioButton : public CheckBox
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		RadioButton(Control *parent = 0);
		
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
		int GetGroup() const;
		
		/**
		 * Verarbeitet ein Event und gibt es wenn n�tig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		virtual IEvent::NextEventTypes ProcessEvent(IEvent *event);
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