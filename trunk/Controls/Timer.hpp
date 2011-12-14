#ifndef OSHGUI_TIMER_HPP_
#define OSHGUI_TIMER_HPP_

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Implementiert einen Zeitgeber, der ein Ereignis in benutzerdefinierten
	 * Intervallen ausl�st.
	 */
	class OSHGUI_EXPORT Timer : public Control
	{
		friend Application;

	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Timer();
		~Timer();
		
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @param isEnabled
		 */
		virtual void SetEnabled(bool isEnabled);
		/**
		 * Legt die Frequenz des Timers in Millisekunden fest.
		 *
		 * @param interval die Frequenz in Millisekunden
		 */
		void SetInterval(int interval);
		/**
		 * Ruft die Frequenz des Timers in Millisekunden ab.
		 *
		 * @return interval
		 */
		long long GetInterval() const;
		/**
		 * Ruft das TickEvent f�r das Steuerelement ab.
		 *
		 * @return tickEvent
		 */
		TickEvent& GetTickEvent();
		
		/**
		 * Startet den Timer. Equivalent zu SetEnabled(true);
		 */
		void Start();
		/**
		 * H�lt den Timer an. Equivalent zu SetEnabled(false);
		 */
		void Stop();

	private:
		int interval;

		TickEvent tickEvent;
	};
}

#endif