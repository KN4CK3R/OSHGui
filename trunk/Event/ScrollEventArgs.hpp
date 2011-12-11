#ifndef OSHGUI_EVENT_SCROLLEVENTARGS_HPP_
#define OSHGUI_EVENT_SCROLLEVENTARGS_HPP_

namespace OSHGui
{
	/**
	 * Stellt Daten für das ScrollEvent bereit.
	 */
	class OSHGUI_EXPORT ScrollEventArgs
	{
	public:
		int NewValue;
		int OldValue;
		
	public:
		ScrollEventArgs(int newValue, int oldValue)
		{
			NewValue = newValue;
			OldValue = oldValue;
		}
	};
}

#endif