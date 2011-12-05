#ifndef OSHGUI_RADIOBUTTON_HPP_
#define OSHGUI_RADIOBUTTON_HPP_

#include "CheckBox.hpp"

namespace OSHGui
{
	/**
	 * Ermöglicht Benutzern bei Verwendung weiterer RadioButton-Steuerelemete
	 * die Auswahl einer Option aus mehreren Auswahlmöglichkeiten.
	 */
	class OSHGUI_EXPORT RadioButton : public CheckBox
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		RadioButton();
		
		/**
		 * Legt den checked-Status fest und passt automatisch
		 * andere RadioButtons in der gleichen Gruppe an.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Legt die Gruppe fest zu der dieser RadioButton gehört.
		 *
		 * @param group
		 */
		void SetGroup(int group);
		/**
		 * Gibt die Gruppe zurück zu der dieser RadioButton gehört.
		 *
		 * @return die Gruppe
		 */
		int GetGroup() const;
		
		/**
		 * Verarbeitet ein Event und gibt es wenn nötig an Kindelemente weiter.
		 *
		 * @param event
		 * @return NextEventTypes
		 */
		//virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		virtual void SetCheckedInternal(bool checked);

		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual bool OnKeyUp(const KeyboardMessage &keyboard);
	
		int group;
	};
}

#endif