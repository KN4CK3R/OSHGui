#ifndef OSHGUI_EVENT_EVENT_HPP
#define OSHGUI_EVENT_EVENT_HPP

#include <list>
#include <functional>

#include "EventHandler.hpp"
#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Event f�r Funktionen mit einem Parameter.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT Event
	{
	private:
		std::list<EventHandler<Signature> > eventHandlers;

	public:		
		/**
		 * Registriert einen EventHandler.
		 *
		 * @param eventHandler
		 */
		Event& operator += (const EventHandler<Signature> &eventHandler)
		{
			eventHandlers.push_back(eventHandler);
			return *this;
		}
		
		/**
		 * Entfernt einen EventHandler.
		 *
		 * @param eventHandler
		 */
		Event& operator -= (const EventHandler<Signature> &eventHandler)
		{
			typename std::list<EventHandler<Signature> >::iterator it = eventHandlers.begin();
			while (it != eventHandlers.end())
			{
				if (*it == eventHandler)
				{
					it = eventHandlers.erase(it);
				}
				else
				{
					++it;
				}
			}
			return *this;
		}
		
		/**
		 * Ruft alle registrierten EventHandler auf.
		 *
		 * @param param1 Funktionsparameter
		 */
		template <typename T>
		void Invoke(T&& param1)
		{
			if (eventHandlers.empty())
			{
				return;
			}

			for (typename std::list<EventHandler<Signature> >::iterator it = eventHandlers.begin(); it != eventHandlers.end(); ++it)
			{
				EventHandler<Signature> &eventHandler = *it;
				eventHandler.GetHandler()(std::forward<T>(param1));
			}
		}

		/**
		 * Ruft alle registrierten EventHandler auf.
		 *
		 * @param param1 Funktionsparameter
		 * @param param2 Funktionsparameter
		 */
		template <typename T, typename T2>
		void Invoke(T&& param1, T2&& param2)
		{
			if (eventHandlers.empty())
			{
				return;
			}

			for (typename std::list<EventHandler<Signature> >::iterator it = eventHandlers.begin(); it != eventHandlers.end(); ++it)
			{
				EventHandler<Signature> &eventHandler = *it;
				eventHandler.GetHandler()(std::forward<T>(param1), std::forward<T2>(param2));
			}
		}
	};
}

#endif