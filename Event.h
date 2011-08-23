#ifndef __OSHGUI_EVENT_H__
#define __OSHGUI_EVENT_H__

namespace OSHGui
{
	class Event
	{
	public:
		enum EventTypes { Mouse, Keyboard, System };
		enum NextEventTypes { DontContinue, Invalidate, Continue };

		EventTypes Type;

		Event(EventTypes Type)
		{
			this->Type = Type;
		}
	};
}

#endif