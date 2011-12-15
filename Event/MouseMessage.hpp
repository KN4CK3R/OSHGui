#ifndef OSHGUI_EVENT_MOUSEMESSAGE_HPP
#define OSHGUI_EVENT_MOUSEMESSAGE_HPP

#include "Drawing\Point.hpp"

namespace OSHGui
{
	/**
	 * Mausnachricht
	 */
	class OSHGUI_EXPORT MouseMessage
	{
	public:
		enum MouseStates
		{
			/**
			 * Unbekannt
			 */
			Unknown,
			/**
			 * Maus wurde bewegt.
			 */
			Move,
			/**
			 * Linke Maustaste wurde gedrückt.
			 */
			LeftDown,
			/**
			 * Linke Maustaste wurde losgelassen.
			 */
			LeftUp,
			/**
			 * Rechte Maustaste wurde gedrückt.
			 */
			RightDown,
			/**
			 * Rechte Maustaste wurde losgelassen.
			 */
			RightUp,
			/**
			 * Mausrad wurde gedreht.
			 */
			Scroll
		};
	
		MouseStates State;
		Drawing::Point Location;
		int Delta;
		
	public:		
		MouseMessage(MouseStates State, Drawing::Point Location, int Delta)
		{
			this->State = State;
			this->Location = Location;
			this->Delta = Delta;
		}
	};
}

#endif