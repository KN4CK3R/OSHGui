#ifndef OSHGUI_EVENT_EVENTHANDLER_HPP_
#define OSHGUI_EVENT_EVENTHANDLER_HPP_

#include <list>
#include <functional>

#include "EventHandler.hpp"
#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * Event für Funktionen mit einem Parameter.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT Event
	{
	private:
		std::list<EventHandler<Signature> > eventFunctions;

	public:		
		/**
		 * Registriert eine Funktion im Event.
		 *
		 * @param eventFunction
		 */
		Event& operator += (const EventHandler<Signature> &eventFunction)
		{
			eventFunctions.push_back(eventFunction);
			return *this;
		}
		
		/**
		 * Entfernt eine Funktion aus dem Event.
		 *
		 * @param eventFunction
		 */
		Event& operator -= (const EventHandler<Signature> &eventFunction)
		{
			typename std::list<EventHandler<Signature> >::iterator it = eventFunctions.begin();
			while (it != eventFunctions.end())
			{
				if (*it == eventFunction)
				{
					it = eventFunctions.erase(it);
				}
				else
				{
					++it;
				}
			}
			return *this;
		}
		
		/**
		 * Ruft alle registrierten Funktionen auf.
		 *
		 * @param param1 Funktionsparameter
		 */
		template <typename T>
		void Invoke(T&& param1)
		{
			for (typename std::list<EventHandler<Signature> >::iterator it = eventFunctions.begin(); it != eventFunctions.end(); ++it)
			{
				EventHandler<Signature> &eventFunction = *it;
				eventFunction.GetHandler()(std::forward<T>(param1));
			}
		}

		/**
		 * Ruft alle registrierten Funktionen auf.
		 *
		 * @param param1 Funktionsparameter
		 * @param param2 Funktionsparameter
		 */
		template <typename T, typename T2>
		void Invoke(T&& param1, T2&& param2)
		{
			for (typename std::list<EventHandler<Signature> >::iterator it = eventFunctions.begin(); it != eventFunctions.end(); ++it)
			{
				EventHandler<Signature> &eventFunction = *it;
				eventFunction.GetHandler()(std::forward<T>(param1), std::forward<T2>(param2));
			}
		}
	};
}

#endif