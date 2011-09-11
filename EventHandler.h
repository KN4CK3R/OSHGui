#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include <list>
#include <memory>

#include "MouseEvent.h"
#include "KeyboardEvent.h"

namespace OSHGui
{
	class Control;

	//VisualStudio 2010 doesn't support Variadic Templates :(

	/**
	 * EventHandler für Funktionen mit einem Parameter.
	 */
	template <typename Type, typename Param1>
	class EventHandlerOneParam
	{
	private:
		class Handler
		{
		public:
			typedef void (Handler::*Type)(Param1);
			
			Handler *source;
			Type function;
		};
		
		std::list<Handler> handlers;

	public:
		/**
		 * Registriert eine Funktion im EventHandler.
		 *
		 * @param source Memberklasse
		 * @param function Funktionszeiger
		 */
		template <typename T>
		void Add(void *source, T function)
		{
			Handler handler;
			handler.source = reinterpret_cast<Handler*>(source);
			handler.function = reinterpret_cast<Handler::Type>(function);
				
			handlers.push_back(handler);
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param source Memberklasse
		 * @param function Funktionszeiger
		 */
		template <typename T>
		void Remove(void *source, T function)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end();)
			{
				Handler &handler = *it;
				if (handler.source == (handler*)source && handler.function == reinterpret_cast<Handler::Type>(function))
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
		void Invoke(Param1 param1)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				Handler &handler = *it;
				(handler.source->*handler.function)(param1);
			}
		}
	};

	template <typename Type, typename Param1, typename Param2>
	class EventHandlerTwoParam
	{
	private:
		class Handler
		{
		public:
			typedef void (Handler::*Type)(Param1, Param2);
			
			Handler *source;
			Type function;
		};
		
		std::list<Handler> handlers;

	public:
		/**
		 * Registriert eine Funktion im EventHandler.
		 *
		 * @param source Memberklasse
		 * @param function Funktionszeiger
		 */
		template <typename T>
		void Add(void *source, T function)
		{
			Handler handler;
			handler.source = reinterpret_cast<Handler*>(source);
			handler.function = reinterpret_cast<Handler::Type>(function);
				
			handlers.push_back(handler);
		}
		
		/**
		 * Entfernt eine Funktion aus dem EventHandler.
		 *
		 * @param source Memberklasse
		 * @param function Funktionszeiger
		 */
		template <typename T>
		void Remove(void *source, T function)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end();)
			{
				Handler &handler = *it;
				if (handler.source == (handler*)source && handler.function == reinterpret_cast<Handler::Type>(function))
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
		void Invoke(Param1 param1, Param2 param2)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				Handler &handler = *it;
				(handler.source->*handler.function)(param1, param2);
			}
		}
	};

	typedef EventHandlerTwoParam<void (*)(const std::shared_ptr<Control>&, const std::shared_ptr<MouseEvent>&), const std::shared_ptr<Control>&, const std::shared_ptr<MouseEvent>&> ClickEventHandler;
	
	typedef EventHandlerTwoParam<void (*)(const std::shared_ptr<Control>&, const std::shared_ptr<KeyboardEvent>&), const std::shared_ptr<Control>&, const std::shared_ptr<KeyboardEvent>&> KeyPressEventHandler;

	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> ChangeEventHandler;
	
	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> MouseEnterEventHandler;
	
	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> MouseLeaveEventHandler;
	
	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> FocusInEventHandler;
	
	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> FocusOutEventHandler;

	typedef EventHandlerOneParam<void (*)(const std::shared_ptr<Control>&), const std::shared_ptr<Control>&> TickEventHandler;
}

#endif