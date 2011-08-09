#ifndef __OSHGUI_SYSTEMEVENT_H__
#define __OSHGUI_SYSTEMEVENT_H__

#include "Event.h"

namespace OSHGui
{
	class SystemEvent : public Event
	{
	public:	
		enum SystemStates { None, ActivateApp, CaptureChanged };
		
		SystemStates State;
		bool Value;
		
		SystemEvent() : Event(Event::System)
		{
			State = None;
			Value = false;
		}
		
		SystemEvent(SystemStates State, bool Value) : Event(Event::System)
		{
			this->State = State;
			this->Value = Value;
		}
	};
}

#endif