#ifndef __OSHGUI_TIMER_H__
#define __OSHGUI_TIMER_H__

#include "Control.h"
#include "Event\EventHandler.h"

namespace OSHGui
{
	/**
	 * Tritt ein, wenn das angegebene Intervall für den Zeitgeber abgelaufen
	 * und der Zeitgeber aktiviert ist.
	 */
	typedef void (*OnTick)(Control *sender);

	/**
	 * Implementiert einen Zeitgeber, der ein Ereignis in benutzerdefinierten
	 * Intervallen auslöst.
	 */
	class Timer : public Control
	{
		friend Application;

	public:
		Timer(Control *parent = 0);
		~Timer();
		
		void SetEnabled(bool enabled);
		bool GetEnabled();
		void SetInterval(long interval);
		long GetInterval();

		/**
		 * Ruft den FocusOutEventHandler für das Steuerelement ab.
		 *
		 * @return forcusOutEventHandler
		 */
		TickEventHandler& GetTickEventHandler();

	protected:
		bool enabled;
		long interval;

		TickEventHandler tickEventHandler;
	};
}

#endif