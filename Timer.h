#ifndef __OSHGUI_TIMER_H__
#define __OSHGUI_TIMER_H__

#include "Control.h"
#include "Event\EventHandler.h"

namespace OSHGui
{
	typedef EventHandler<void(Control*)> TickEvent;

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
		bool GetEnabled() const;
		void SetInterval(long interval);
		long GetInterval() const;

		/**
		 * Ruft das TickEvent für das Steuerelement ab.
		 *
		 * @return tickEvent
		 */
		TickEvent& GetTickEvent();

	protected:
		bool enabled;
		long interval;

		TickEvent tickEvent;
	};
}

#endif