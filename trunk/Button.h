#ifndef __OSHGUI_BUTTON_H__
#define __OSHGUI_BUTTON_H__

#include "Label.h"
#include "Event\KeyEventArgs.h"
#include "Event\KeyPressEventArgs.h"

namespace OSHGui
{
	/**
	 * Tritt ein, wenn eine Taste gedr�ckt wird.
	 */
	typedef EventHandler<void(Control*, const KeyEventArgs&)> KeyDownEvent;
	/**
	 * Tritt ein, wenn das Steuerelement fokusiert ist und eine Taste gedr�ckt gehalten wird.
	 */
	typedef EventHandler<void(Control*, const KeyPressEventArgs&)> KeyPressEvent;
	/**
	 * Tritt ein, wenn eine Taste losgelassen wird.
	 */
	typedef EventHandler<void(Control*, const KeyEventArgs&)> KeyUpEvent;

	/**
	 * Stellt ein Schaltfl�chen-Steuerelement dar.
	 */
	class Button : public Label
	{
	public:
		/**
		 * Konstruktor der Klasse.
		 *
		 * @param parent das Elternsteuerelement
		 */
		Button(Control *parent = 0);
		virtual ~Button();
		
		/**
		 * Ruft das KeyDownEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyDownEvent& GetKeyDownEvent();
		/**
		 * Ruft das KeyPressEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyPressEvent& GetKeyPressEvent();
		/**
		 * Ruft das KeyUpEvent f�r das Steuerelement ab.
		 *
		 * @return keyPressEvent
		 */
		KeyUpEvent& GetKeyUpEvent();
		
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
		KeyDownEvent keyDownEvent;
		KeyPressEvent keyPressEvent;
		KeyUpEvent keyUpEvent;
	};
}

#endif