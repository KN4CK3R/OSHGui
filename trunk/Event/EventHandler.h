#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include <list>
#include <memory>
#include <functional>

#include "MouseEvent.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	class Control;

	/**
	 * EventHandler f�r Funktionen mit einem Parameter.
	 */
	template <typename Signature>
	class EventHandler
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
		void Add(Handler const& handler)
		{		
			handlers.push_back(handler);
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param handler
		 */
		void Remove(Handler const& handler)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end();)
			{
				if (*it == handler)
				{
					it = handlers.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		
		/**
		 * Ruft alle registrierten Funktionen auf.
		 *
		 * @param param1 Funktionsparameter
		 */
		template <typename T>
		void Invoke(T&& param1)
		{
			for (auto it = handlers.begin(); it != handlers.end(); ++it)
			{
				Handler &handler = *it;
				handler(std::forward<T>(param1));
			}
		}

		template <typename T, typename T2>
		void Invoke(T&& param1, T2&& param2)
		{
			for (auto it = handlers.begin(); it != handlers.end(); ++it)
			{
				Handler &handler = *it;
				handler(std::forward<T>(param1), std::forward<T2>(param2));
			}
		}
	};

	typedef EventHandler<void(Control*, MouseEvent*)> ClickEventHandler;
	
	typedef EventHandler<void(Control*, KeyboardEvent*)> KeyPressEventHandler;

	typedef EventHandler<void(Control*)> ChangeEventHandler;
	
	typedef EventHandler<void(Control*)> MouseEnterEventHandler;
	
	typedef EventHandler<void(Control*)> MouseLeaveEventHandler;
	
	typedef EventHandler<void(Control*)> FocusInEventHandler;
	
	typedef EventHandler<void(Control*)> FocusOutEventHandler;

	typedef EventHandler<void(Control*)> TickEventHandler;
}

#endif