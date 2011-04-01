#ifndef __PAINTMESSAGE_H__
#define __PAINTMESSAGE_H__

namespace OSHGui
{
	class PaintEvent : public Event
	{
	public:	
		Rectangle Clip;
		Graphics *Graphic;
		
	public:
		PaintEvent(Rectangle Clip, Graphics *Graphic) : Event(Event::Paint)
		{
			this->Clip = Clip;
			this->Graphic = Graphic;
		}
	};
}

#endif