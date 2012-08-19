/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TIMERMANAGER_HPP
#define OSHGUI_TIMERMANAGER_HPP

#include <vector>
#include "Misc\DateTime.hpp"
#include "Misc\TimeSpan.hpp"
#include "Exports.hpp"

namespace OSHGui
{
	class Timer;

	class OSHGUI_EXPORT TimerManager
	{
	public:
		/**
		 * Registriert den Timer mit dem entsprechenden Interval.
		 *
		 * @param timer der Timer
		 * @param interval das Interval
		 */
		void RegisterTimer(Timer *timer, const Misc::TimeSpan &interval);
		/**
		 * Entfernt den Timer.
		 *
		 * @param timer der Timer
		 */
		void UnregisterTimer(Timer *timer);
		
		/**
		 * Updatet die Timer.
		 */
		void Update();

	private:
		struct TimerInfo
		{
			Timer *timer;
			Misc::TimeSpan interval;
			Misc::DateTime next;
			bool remove;
		};
		std::vector<TimerInfo> timers;
	};
}

#endif