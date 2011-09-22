#ifndef __OSHGUI_TIMER_H__
#define __OSHGUI_TIMER_H__

#include "Control.h"
#include "Event\EventHandler.h"

namespace OSHGui
{
	typedef EventHandler<void(Control*)> TickEventHandler;

	/**
	 * Implementiert einen Zeitgeber, der ein Ereignis in benutzerdefinierten
	 * Intervallen ausl�st.
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
		 * Ruft den FocusOutEventHandler f�r das Steuerelement ab.
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