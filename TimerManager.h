#ifndef OSHGUI_TIMERMANAGER_H_
#define OSHGUI_TIMERMANAGER_H_

#include <list>
#include "Misc\DateTime.h"
#include "Misc\TimeSpan.h"

namespace OSHGui
{
	class Timer;

	class TimerManager
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