#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include <list>

namespace OSHGui
{
	class Control;
	/**
	 * Tritt beim Klicken auf das Steuerelement ein.
	 */
	typedef void (*OnClickFunc)(Control *sender, MouseEvent *mouse);
	/**
	 * Tritt ein, wenn eine Taste gedrückt wird, während das Steuerelement den Fokus hat.
	 */
	typedef void (*OnKeyPressFunc)(Control *sender, KeyboardEvent *keyboard);
	/**
	 * Tritt ein, wenn der Mauszeiger den Bereich des Steuerelements betritt.
	 */
	typedef void (*OnEnterFunc)(Control *sender);
	/**
	 * Tritt ein, wenn der Mauszeiger den Bereich des Steuerelements verlässt.
	 */
	typedef void (*OnLeaveFunc)(Control *sender);
	/**
	 * Tritt ein, wenn der Wert des Steuerelements verändert wird.
	 */
	typedef void (*OnChangeFunc)(Control *sender);

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
		template <typename T>
		void Add(void *source, T function)
		{
			Handler handler;
			handler.source = reinterpret_cast<Handler*>(source);
			handler.function = reinterpret_cast<Handler::Type>(function);
				
			handlers.push_back(handler);
		}
		
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
		template <typename T>
		void Add(void *source, T function)
		{
			Handler handler;
			handler.source = reinterpret_cast<Handler*>(source);
			handler.function = reinterpret_cast<Handler::Type>(function);
				
			handlers.push_back(handler);
		}
		
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
		
		void Invoke(Param1 param1, Param2 param2)
		{
			for (std::list<Handler>::iterator it = handlers.begin(); it != handlers.end(); it++)
			{
				Handler &handler = *it;
				(handler.source->*handler.function)(param1, param2);
			}
		}
	};

	typedef EventHandlerTwoParam<OnClickFunc, Control*, MouseEvent*> ClickEventHandler;
	
	typedef EventHandlerTwoParam<OnKeyPressFunc, Control*, KeyboardEvent*> KeyPressEventHandler;

	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> ChangeEventHandler;
	
	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> MouseEnterEventHandler;
	
	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> MouseLeaveEventHandler;
	
	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> FocusInEventHandler;
	
	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> FocusOutEventHandler;

	typedef EventHandlerOneParam<OnKeyPressFunc, Control*> TickEventHandler;
}

#endif