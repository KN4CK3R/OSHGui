#ifndef __OSHGUI_MISC_TIMER_H__
#define __OSHGUI_MISC_TIMER_H__

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <sys/time.h>
#include <map>

namespace OSHGui
{
	class Timer;

	namespace Misc
	{
		class TimeHelper
		{
		public:
			TimeHelper();
			
			void Reset();
			void Start();
			void Stop();
			void Advance();
			double GetAbsoluteTime();
			double GetTime();
			float GetElapsedTime();
			bool IsStopped();
			
			void RegisterTimer(Timer *timer, unsigned long interval);
			void UnregisterTimer(Timer *timer); 

		private:
			LARGE_INTEGER GetAdjustedCurrentTime();

			bool usingQPF;
			bool timerStopped;
			LONGLONG qpfTicksPerSec;

			LONGLONG stopTime;
			LONGLONG lastElapsedTime;
			LONGLONG baseTime;
			
			struct TimerInfo
			{
				Timer *timer;
				unsigned long intervalInMicro;
			}
			
			std::map<Timer*, TimerInfo> timers;
		};

		extern TimeHelper GlobalTime;
	}
}

#endif