#ifndef __OSHGUI_MISC_TIMER_H__
#define __OSHGUI_MISC_TIMER_H__

#include <windows.h>

namespace OSHGui
{
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

		protected:
			LARGE_INTEGER GetAdjustedCurrentTime();

			bool usingQPF;
			bool timerStopped;
			LONGLONG qpfTicksPerSec;

			LONGLONG stopTime;
			LONGLONG lastElapsedTime;
			LONGLONG baseTime;
		};
	}
}

#endif