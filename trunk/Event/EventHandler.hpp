#ifndef OSHGUI_EVENT_EVENTHANDLER_HPP_
#define OSHGUI_EVENT_EVENTHANDLER_HPP_

#include <list>
#include <functional>

#include "EventFunction.hpp"
#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * EventHandler für Funktionen mit einem Parameter.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT EventHandler
	{
	private:
		std::list<EventFunction<Signature> > eventFunctions;

	public:
		/**
		 * Registriert eine Funktion im EventHandler.
		 *
		 * @param eventFunction
		 */
		void Add(const EventFunction<Signature> &eventFunction)
		{
			eventFunctions.push_back(eventFunction);
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param eventFunction
		 */
		void Remove(const EventFunction<Signature> &eventFunction)
		{
			typename std::list<EventFunction<Signature> >::iterator it = eventFunctions.begin();
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
		}
		
		/**
		 * Registriert eine Funktion im EventHandler.
		 *
		 * @param eventFunction
		 */
		EventHandler& operator += (const EventFunction<Signature> &eventFunction)
		{
			Add(eventFunction);
			return *this;
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param eventFunction
		 */
		EventHandler& operator -= (const EventFunction<Signature> &eventFunction)
		{
			Remove(eventFunction);
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
			for (typename std::list<EventFunction<Signature> >::iterator it = eventFunctions.begin(); it != eventFunctions.end(); ++it)
			{
				EventFunction<Signature> &eventFunction = *it;
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
			for (typename std::list<EventFunction<Signature> >::iterator it = eventFunctions.begin(); it != eventFunctions.end(); ++it)
			{
				EventFunction<Signature> &eventFunction = *it;
				eventFunction.GetHandler()(std::forward<T>(param1), std::forward<T2>(param2));
			}
		}
	};
}

#endif