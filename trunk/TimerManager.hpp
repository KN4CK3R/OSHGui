/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_TIMERMANAGER_HPP
#define OSHGUI_TIMERMANAGER_HPP

#include <list>
#include "Misc\DateTime.hpp"
#include "Misc\TimeSpan.hpp"
#include "Exports.hpp"

namespace OSHGui
{
	class Timer;

	class OSHGUI_EXPORT TimerManager
	{
	public:
		void RegisterTimer(Timer *timer, const Misc::TimeSpan &interval);
		void UnregisterTimer(Timer *timer);
		void Update();

	private:
		struct TimerInfo
		{
			Timer *timer;
			Misc::TimeSpan interval;
			Misc::DateTime next;
			bool remove;
		};
		std::list<TimerInfo> timers;
	};
}

#endif