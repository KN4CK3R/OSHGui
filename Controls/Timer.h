#ifndef OSHGUI_TIMER_H_
#define OSHGUI_TIMER_H_

#include "Control.h"
#include "Event\EventHandler.h"

namespace OSHGui
{
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
		
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @param enabled
		 */
		void SetEnabled(bool enabled);
		/**
		 * Ruft ab, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @return enabled
		 */
		bool GetEnabled() const;
		/**
		 * Legt die Frequenz des Timers in Millisekunden fest.
		 *
		 * @param interval die Frequenz in Millisekunden
		 */
		void SetInterval(long interval);
		/**
		 * Ruft die Frequenz des Timers in Millisekunden ab.
		 *
		 * @return interval
		 */
		long GetInterval() const;
		/**
		 * Ruft das TickEvent für das Steuerelement ab.
		 *
		 * @return tickEvent
		 */
		TickEvent& GetTickEvent();
		
		/**
		 * Startet den Timer. Equivalent zu SetEnabled(true);
		 */
		void Start();
		/**
		 * Hält den Timer an. Equivalent zu SetEnabled(false);
		 */
		void Stop();

	protected:
		bool enabled;
		long interval;

		TickEvent tickEvent;
	};
}

#endif