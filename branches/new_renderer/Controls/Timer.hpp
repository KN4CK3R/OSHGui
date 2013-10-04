/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TIMER_HPP
#define OSHGUI_TIMER_HPP

#include "Control.hpp"

namespace OSHGui
{
	/**
	 * Tritt auf, wenn das angegebene Intervall verstrichen ist.
	 */
	typedef Event<void(Control*)> TickEvent;
	typedef EventHandler<void(Control*)> TickEventHandler;

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
		virtual ~Timer();
		
		/**
		 * Legt fest, ob das Steuerlement auf Benutzerinteraktionen reagieren kann.
		 *
		 * @param isEnabled
		 */
		virtual void SetEnabled(bool isEnabled) override;
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
		/**
		 * �berpr�ft, ob sich der Punkt innerhalb des Steuerelements befindet.
		 *
		 * @param point
		 * @return ja / nein
		 */
		virtual bool Intersect(const Drawing::Point &point) const override;

	private:
		int interval;

		TickEvent tickEvent;
	};
}

#endif