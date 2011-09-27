#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include <list>
#include <memory>
#include <functional>

#include "MouseEvent.h"
#include "MouseEventArgs.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	class Control;

	/**
	 * EventHandler für Funktionen mit einem Parameter.
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
					++it;
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
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); ++it)
			{
				Handler &handler = *it;
				handler(std::forward<T>(param1));
			}
		}

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

	typedef EventHandler<void(Control*)> ClickEvent;
		
	typedef EventHandler<void(Control*, const MouseEventArgs&)> MouseClickEvent;

	typedef EventHandler<void(Control*, const MouseEventArgs&)> MouseDownEvent;

	typedef EventHandler<void(Control*, const MouseEventArgs&)> MouseMoveEvent;

	typedef EventHandler<void(Control*, const MouseEventArgs&)> MouseUpEvent;
	
	typedef EventHandler<void(Control*)> MouseEnterEvent;
	
	typedef EventHandler<void(Control*)> MouseLeaveEvent;
	
	typedef EventHandler<void(Control*)> FocusInEvent;
	
	typedef EventHandler<void(Control*)> FocusOutEvent;
}

#endif