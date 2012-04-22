/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TIMER_HPP
#define OSHGUI_TIMER_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Implementiert einen Zeitgeber, der ein Ereignis in benutzerdefinierten
	 * Intervallen auslöst.
	 */
	class OSHGUI_EXPORT Timer : public Control
	{
		friend Application;

	public:
		/**
		 * Konstruktor der Klasse.
		 */
		Timer();
		virtual ~Timer();
		
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
		/**
		 * Überprüft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const;

	private:
		int interval;

		TickEvent tickEvent;
	};
}

#endif