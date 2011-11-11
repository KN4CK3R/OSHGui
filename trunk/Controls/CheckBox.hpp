#ifndef OSHGUI_CHECKBOX_HPP_
#define OSHGUI_CHECKBOX_HPP_

#include "Label.hpp"

namespace OSHGui
{
	/**
	 * Stellt ein CheckBox-Steuerelement dar.
	 */
	class OSHGUI_EXPORT CheckBox : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		CheckBox(Control *parent);
		
		/**
		 * Legt den checked-Status fest.
		 *
		 * @param checked
		 */
		virtual void SetChecked(bool checked);
		/**
		 * Gibt den checked-Status zurück.
		 *
		 * @return ja / nein
		 */
		virtual bool GetChecked() const;
		/**
		 * Ruft das CheckedChangeEvent für das Steuerelement ab.
		 *
		 * @return checkedChangeEvent
		 */
		CheckedChangedEvent& GetCheckedChangedEvent();
		
		/**
		 * Überprüft, ob das Steuerelement den Fokus übernehmen kann.
		 *
		 * @return ja / nein
		 */
		virtual bool CanHaveFocus() const;

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
		//virtual bool ProcessEvent(IEvent *event);
		/**
		 * Zeichnet das Steuerelement mithilfe des übergebenen IRenderers.
		 *
		 * @param renderer
		 */
		virtual void Render(Drawing::IRenderer *renderer);
	
	protected:
		virtual void OnMouseClick(const MouseEvent &mouse);
		virtual void OnKeyUp(const KeyboardEvent &keyboard);

		bool checked;
		Drawing::Point checkBoxPosition,
					   textPosition;
					   
		CheckedChangedEvent checkedChangedEvent;
	};
}

#endif