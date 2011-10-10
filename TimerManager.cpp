#include "TimerManager.h"
#include "Controls\Timer.h"
#include "Misc\Exceptions.h"
#include "Application.h"

namespace OSHGui
{
	void TimerManager::RegisterTimer(Timer *timer, const Misc::TimeSpan &interval)
	{
		if (timer == 0)
		{
			throw Misc::ArgumentNullException(L"timer", __WFILE__, __LINE__);
		}
	
		bool found = false;
		for (std::list<TimerInfo>::iterator it = timers.begin(); it != timers.end(); ++it)
		{
			if ((*it).timer == timer)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			TimerInfo info;
			info.timer = timer;
			info.interval = interval;
			info.next = Application::GetNow().Add(interval);
			timers.push_back(info);
		}
	}

	void TimerManager::UnregisterTimer(Timer *timer)
	{
		if (timer == 0)
		{
			throw Misc::ArgumentNullException(L"timer", __WFILE__, __LINE__);
		}
	
		for (std::list<TimerInfo>::iterator it = timers.begin(); it != timers.end(); ++it)
		{
			TimerInfo info = *it;
			if (info.timer == timer)
			{
				timers.erase(it);
				return;
			}
		}
	}

	void TimerManager::Update()
	{
		if (timers.size() > 0)
		{
			for (std::list<TimerInfo>::iterator it = timers.begin(); it != timers.end(); ++it)
			{
				TimerInfo info = *it;
				if (info.next < Application::GetNow())
				{
					info.timer->GetTickEvent().Invoke(info.timer);
					info.next = Application::GetNow().Add(info.interval);
				}
			}
		}
	}
}