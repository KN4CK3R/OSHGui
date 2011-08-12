#include "TimeHelper.h"

namespace OSHGui
{
	namespace Misc
	{
		TimeHelper GlobalTime;

		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TimeHelper::TimeHelper()
		{
			timerStopped = true;
			qpfTicksPerSec = 0;

			stopTime = 0;
			lastElapsedTime = 0;
			baseTime = 0;

			LARGE_INTEGER ticksPerSec = { 0 };
			QueryPerformanceFrequency(&ticksPerSec);
			qpfTicksPerSec = ticksPerSec.QuadPart;
			
			HANDLE currentProcess = GetCurrentProcess();

			DWORD_PTR processAffinityMask = 0;
			DWORD_PTR systemAffinityMask = 0;

			if (GetProcessAffinityMask(currentProcess, &processAffinityMask, &systemAffinityMask) != NULL && processAffinityMask != NULL)
			{
				DWORD_PTR affinityMask = (processAffinityMask & ((~processAffinityMask) + 1));

				HANDLE currentThread = GetCurrentThread();
				if (currentThread != INVALID_HANDLE_VALUE)
				{
					SetThreadAffinityMask(currentThread, affinityMask);
					CloseHandle(currentThread);
				}
			}

			CloseHandle(currentProcess);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void TimeHelper::Reset()
		{
			LARGE_INTEGER temp = GetAdjustedCurrentTime();

			baseTime = temp.QuadPart;
			lastElapsedTime = temp.QuadPart;
			stopTime = 0;
			timerStopped = false;
		}
		//---------------------------------------------------------------------------
		void TimeHelper::Start()
		{
			LARGE_INTEGER temp = { 0 };
			QueryPerformanceCounter(&temp);

			if (timerStopped)
			{
				baseTime += temp.QuadPart - stopTime;
			}
			stopTime = 0;
			lastElapsedTime = temp.QuadPart;
			timerStopped = false;
		}
		//---------------------------------------------------------------------------
		void TimeHelper::Stop()
		{
			if (!timerStopped)
			{
				LARGE_INTEGER temp = { 0 };
				QueryPerformanceCounter(&temp);
				stopTime = temp.QuadPart;
				lastElapsedTime = temp.QuadPart;
				timerStopped = true;
			}
		}
		//---------------------------------------------------------------------------
		void TimeHelper::Advance()
		{
			stopTime += qpfTicksPerSec / 10;
		}
		//---------------------------------------------------------------------------
		double TimeHelper::GetAbsoluteTime()
		{
			LARGE_INTEGER temp = { 0 };
			QueryPerformanceCounter(&temp);

			double ret = temp.QuadPart / (double)qpfTicksPerSec;

			return ret;
		}
		//---------------------------------------------------------------------------
		double TimeHelper::GetTime()
		{
			LARGE_INTEGER temp = GetAdjustedCurrentTime();

			double ret = (double)(temp.QuadPart - baseTime) / (double)qpfTicksPerSec;

			return ret;
		}
		//---------------------------------------------------------------------------
		float TimeHelper::GetElapsedTime()
		{
			LARGE_INTEGER temp = GetAdjustedCurrentTime();

			float elapsedTime = (float)((double)(temp.QuadPart - lastElapsedTime) / (double)qpfTicksPerSec);
			lastElapsedTime = temp.QuadPart;

			if (elapsedTime < 0.0f)
			{
				elapsedTime = 0.0f;
			}

			return elapsedTime;
		}
		//---------------------------------------------------------------------------
		bool TimeHelper::IsStopped()
		{
			return timerStopped;
		}
		//---------------------------------------------------------------------------
		LARGE_INTEGER TimeHelper::GetAdjustedCurrentTime()
		{
			LARGE_INTEGER temp = { 0 };
			if (stopTime != 0)
			{
				temp.QuadPart = stopTime;
			}
			else
			{
				QueryPerformanceCounter(&temp);
			}
			return temp;
		}
		//---------------------------------------------------------------------------
	}
}