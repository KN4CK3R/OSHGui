#ifndef __OSHGUI_TIMER_H__
#define __OSHGUI_TIMER_H__

#include <windows.h>
#include <map>

namespace OSHGui
{
	/**
	 * Tritt ein, wenn das angegebene Intervall für den Zeitgeber abgelaufen
	 * und der Zeitgeber aktiviert ist.
	 */
	typedef void (*OnTick)(Control *sender);

	/**
	 * Implementiert einen Zeitgeber, der ein Ereignis in benutzerdefinierten
	 * Intervallen auslöst.
	 */
	class Timer
	{
	public:
		Timer();
		~Timer();
		
		void SetEnabled(bool enabled);
		bool GetEnabled();
		void SetInterval(int interval);
		int GetInterval();
		void SetOnTickFunc(OnTick onTickFunc);

		void OnTimer();

	protected:
		static std::map<UINT_PTR, Timer*> timerTable;
		static void CALLBACK TimerCallback(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime);
		UINT_PTR timerId;

		bool enabled;
		int interval;
		OnTick onTickFunc;
	};
}

#endif