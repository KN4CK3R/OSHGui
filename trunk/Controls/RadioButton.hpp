#ifndef OSHGUI_RADIOBUTTON_HPP
#define OSHGUI_RADIOBUTTON_HPP

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
		 * Zeichnet das Steuerelement mithilfe des �bergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
		
	protected:
		virtual void SetCheckedInternal(bool checked);

		virtual void OnMouseClick(const MouseMessage &mouse);
		virtual bool OnKeyUp(const KeyboardMessage &keyboard);
	};
}

#endif