#ifndef OSHGUI_EVENT_EVENTHANDLER_HPP_
#define OSHGUI_EVENT_EVENTHANDLER_HPP_

#include <list>
#include <memory>
#include <functional>

#include "../Exports.hpp"

namespace OSHGui
{
	/**
	 * EventHandler f�r Funktionen mit einem Parameter.
	 */
	template <typename Signature>
	class OSHGUI_EXPORT EventHandler
	{
	private:
		typedef std::function<Signature> Handler;
		
		std::list<Handler> handlers;

	public:
		/**
		 * Registriert eine Funktion im EventHandler.
		 *
		 * @param handler
		 */
		void Add(const Handler &handler)
		{
			handlers.push_back(handler);
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param handler
		 */
		void Remove(const Handler &handler)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end();)
			{
				if (*it == handler)
				{
					it = handlers.erase(it);
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
		 * @param handler
		 */
		EventHandler& operator += (const Handler &handler)
		{
			Add(handler);
			return *this;
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param handler
		 */
		EventHandler& operator -= (const Handler &handler)
		{
			Remove(handler);
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
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); ++it)
			{
				Handler &handler = *it;
				handler(std::forward<T>(param1));
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
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); ++it)
			{
				Handler &handler = *it;
				handler(std::forward<T>(param1), std::forward<T2>(param2));
			}
		}
	};
}

#endif