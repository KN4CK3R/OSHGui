#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include <list>

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

	typedef EventHandlerTwoParam<void (*)(Control*, MouseEvent*), Control*, MouseEvent*> ClickEventHandler;
	
	typedef EventHandlerTwoParam<void (*)(Control*, KeyboardEvent*), Control*, KeyboardEvent*> KeyPressEventHandler;

	typedef EventHandlerOneParam<void (*)(Control*), Control*> ChangeEventHandler;
	
	typedef EventHandlerOneParam<void (*)(Control*), Control*> MouseEnterEventHandler;
	
	typedef EventHandlerOneParam<void (*)(Control*), Control*> MouseLeaveEventHandler;
	
	typedef EventHandlerOneParam<void (*)(Control*), Control*> FocusInEventHandler;
	
	typedef EventHandlerOneParam<void (*)(Control*), Control*> FocusOutEventHandler;

	typedef EventHandlerOneParam<void (*)(Control*), Control*> TickEventHandler;
}

#endif