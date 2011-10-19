#ifndef OSHGUI_TIMERMANAGER_HPP_
#define OSHGUI_TIMERMANAGER_HPP_

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
		};
		std::list<TimerInfo> timers;
	};
}

#endif