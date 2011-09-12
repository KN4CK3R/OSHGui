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

	typedef EventHandler<void(const std::shared_ptr<Control>&, const std::shared_ptr<MouseEvent>&)> ClickEventHandler;
	
	typedef EventHandler<void(const std::shared_ptr<Control>&, const std::shared_ptr<KeyboardEvent>&)> KeyPressEventHandler;

	typedef EventHandler<void(const std::shared_ptr<Control>&)> ChangeEventHandler;
	
	typedef EventHandler<void(const std::shared_ptr<Control>&)> MouseEnterEventHandler;
	
	typedef EventHandler<void(const std::shared_ptr<Control>&)> MouseLeaveEventHandler;
	
	typedef EventHandler<void(const std::shared_ptr<Control>&)> FocusInEventHandler;
	
	typedef EventHandler<void(const std::shared_ptr<Control>&)> FocusOutEventHandler;

	typedef EventHandler<void(const std::shared_ptr<Control>&)> TickEventHandler;
}

#endif