/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TimerManager.hpp"
#include "Controls/Timer.hpp"
#include "Misc/Exceptions.hpp"
#include "Application.hpp"

namespace OSHGui
{
	void TimerManager::RegisterTimer(Timer *timer, const Misc::TimeSpan &interval)
	{
		if (timer == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("timer", __FILE__, __LINE__);
			#endif
			return;
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
			info.remove = false;
			info.timer = timer;
			info.interval = interval;
			info.next = Application::Instance()->GetNow().Add(interval);
			timers.push_back(info);
		}
	}
	//---------------------------------------------------------------------------
	void TimerManager::UnregisterTimer(Timer *timer)
	{
		if (timer == nullptr)
		{
			#ifndef OSHGUI_DONTUSEEXCEPTIONS
			throw Misc::ArgumentNullException("timer", __FILE__, __LINE__);
			#endif
			return;
		}
	
		for (std::list<TimerInfo>::iterator it = timers.begin(); it != timers.end(); ++it)
		{
			TimerInfo &info = *it;
			if (info.timer == timer)
			{
				info.remove = true;
				return;
			}
		}
	}
	//---------------------------------------------------------------------------
	void TimerManager::Update()
	{
		if (timers.size() > 0)
		{
			Application *app = Application::Instance();
			for (std::list<TimerInfo>::iterator it = timers.begin(); it != timers.end(); )
			{
				TimerInfo &info = *it;
				if (!info.remove && info.next < app->GetNow())
				{
					info.timer->GetTickEvent().Invoke(info.timer);
					info.next = app->GetNow().Add(info.interval);
				}
				if (info.remove)
				{
					it = timers.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
	//---------------------------------------------------------------------------
}