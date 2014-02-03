/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
		Handler handler_;
		int id_;
		
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
		 * \param handler die gebundene Funktion
		 */
		EventHandler(const Handler &handler)
			: handler_(handler),
			  id_(NextID())
		{
			
		}
		
		/**
		 * CopyKonstruktor der Klasse.
		 *
		 * \param eventHandler
		 */
		EventHandler(const EventHandler &eventHandler)
			: id_(eventHandler.id_),
			  handler_(eventHandler.handler_)
		{
			
		}

		bool operator == (const EventHandler &eventHandler)
		{
			return id_ == eventHandler.id_;
		}
		
		EventHandler& operator = (const EventHandler &eventHandler)
		{
			if (this != &eventHandler)
			{
				id_ = eventHandler.id_;
				handler_ = eventHandler.handler_;
			}
			return *this;
		}

		/**
		 * Ruft die gebundene Funktion ab.
		 *
		 * \return handler
		 */
		Handler& GetHandler()
		{
			return handler_;
		}
	};
}

#endif