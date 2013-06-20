/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_EVENTHANDLER_HPP
#define OSHGUI_EVENT_EVENTHANDLER_HPP

#include <functional>
#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Stellt die Methode dar, die ein Ereignis behandelt.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT EventHandler
	{
	private:
		typedef std::function<Signature> Handler;
		Handler handler;
		int id;
		
		static unsigned int NextID()
		{
			static unsigned int id = 0;
			++id;
			return id;
		}
		
	public:
		/**
		 * Konstruktor der Klasse.
		 */
		EventHandler()
		{

		}

		/**
		 * Konstruktor der Klasse.
		 *
		 * @param handler die gebundene Funktion
		 */
		EventHandler(const Handler &handler)
		{
			id = NextID();
			this->handler = handler;
		}
		
		/**
		 * CopyKonstruktor der Klasse.
		 *
		 * @param eventHandler
		 */
		EventHandler(const EventHandler &eventHandler)
		{
			id = eventHandler.id;
			handler = eventHandler.handler;
		}

		bool operator == (const EventHandler &eventHandler)
		{
			return id == eventHandler.id;
		}
		
		EventHandler& operator = (const EventHandler &eventHandler)
		{
			if (this != &eventHandler)
			{
				id = eventHandler.id;
				handler = eventHandler.handler;
			}
			return *this;
		}

		/**
		 * Ruft die gebundene Funktion ab.
		 *
		 * @return handler
		 */
		Handler& GetHandler()
		{
			return handler;
		}
	};
}

#endif