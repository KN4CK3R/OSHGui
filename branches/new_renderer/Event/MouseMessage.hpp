/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_MOUSEMESSAGE_HPP
#define OSHGUI_EVENT_MOUSEMESSAGE_HPP

#include "../Drawing/Point.hpp"
#include "MouseButton.hpp"
#include "../Misc/ConcurrendBoundedBuffer.hpp"

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
			 * Maustaste wurde gedrückt.
			 */
			Down,
			/**
			 * Maustaste wurde losgelassen.
			 */
			Up,
			/**
			 * Mausrad wurde gedreht.
			 */
			Scroll
		};
	
		MouseStates State;
		MouseButton::MouseButtons Button;
		Drawing::PointF Location;
		int Delta;
		
	public:
		/**
		 * Konstruktor der Klasse
		 */
		MouseMessage()
			: state(Unknown),
			  button(MouseButton::None),
			  delta(0)
		{

		}

		MouseMessage(MouseStates state, MouseButton::MouseButtons button, Drawing::PointF location, int delta)
		{
			this->State = state;
			this->Button = button;
			this->Location = location;
			this->Delta = delta;
		}

	private:
		MouseStates state;
		MouseButton::MouseButtons button;
		Drawing::PointF location;
		int delta;
	};

	typedef Misc::BoundedBuffer<MouseMessage, 20> MouseMessageQueue;
}

#endif