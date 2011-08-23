#ifndef __OSHGUI_EVENTHANDLER_H__
#define __OSHGUI_EVENTHANDLER_H__

#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include <list>

namespace OSHGui
{
	class Control;
	
	class ClickEventHandler
	{
	private:
		class ClickHandler
		{
		public:
			typedef void (ClickHandler::*OnClickFunc)(Control *sender, MouseEvent *mouse);
			
			ClickHandler *source;
			OnClickFunc function;
		};
		
		std::list<ClickHandler> handler;

	public:
		template <typename T>
		void Add(void *source, T function)
		{
			ClickHandler clickHandler;
			clickHandler.source = reinterpret_cast<ClickHandler*>(source);
			clickHandler.function = reinterpret_cast<ClickHandler::OnClickFunc>(function);
				
			handler.push_back(clickHandler);
		}
		
		template <typename T>
		void Remove(void *source, T function)
		{
			for (std::list<ClickHandler>::iterator it = handler.begin(); it != handler.end();)
			{
				ClickHandler &clickHandler = *it;
				if (clickHandler.source == (ClickHandler*)source && clickHandler.function == reinterpret_cast<ClickHandler::OnClickFunc>(function))
				{
					it = handler.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		
		void Invoke(Control *sender, MouseEvent *mouse)
		{
			for (std::list<ClickHandler>::iterator it = handler.begin(); it != handler.end(); it++)
			{
				ClickHandler &clickHandler = *it;
				(clickHandler.source->*clickHandler.function)(sender, mouse);
			}
		}
	};
	
	class KeyPressEventHandler
	{
	private:
		class KeyPressHandler
		{
		public:
			typedef void (KeyPressHandler::*OnKeyPressFunc)(Control *sender, KeyboardEvent *mouse);
			
			KeyPressHandler *source;
			OnKeyPressFunc function;
		};
		
		std::list<KeyPressHandler> handler;

	public:
		template <typename T>
		void Add(void *source, T function)
		{
			KeyPressHandler keyPressHandler;
			keyPressHandler.source = reinterpret_cast<KeyPressHandler*>(source);
			keyPressHandler.function = reinterpret_cast<KeyPressHandler::OnKeyPressFunc>(function);
				
			handler.push_back(keyPressHandler);
		}
		
		template <typename T>
		void Remove(void *source, T function)
		{
			for (std::list<KeyPressHandler>::iterator it = handler.begin(); it != handler.end();)
			{
				KeyPressHandler &keyPressHandler = *it;
				if (keyPressHandler.source == (KeyPressHandler*)source && keyPressHandler.function == reinterpret_cast<KeyPressHandler::OnClickFunc>(function))
				{
					it = handler.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		
		void Invoke(Control *sender, KeyboardEvent *keyboard)
		{
			for (std::list<KeyPressHandler>::iterator it = handler.begin(); it != handler.end(); it++)
			{
				KeyPressHandler &keyPressHandler = *it;
				(keyPressHandler.source->*keyPressHandler.function)(sender, keyboard);
			}
		}
	};

	class ChangeEventHandler
	{
	private:
		class ChangeHandler
		{
		public:
			typedef void (ChangeHandler::*OnChangeFunc)(Control *sender);
			
			ChangeHandler *source;
			OnChangeFunc function;
		};
		
		std::list<ChangeHandler> handler;

	public:
		template <typename T>
		void Add(void *source, T function)
		{
			ChangeHandler changeHandler;
			changeHandler.source = reinterpret_cast<ChangeHandler*>(source);
			changeHandler.function = reinterpret_cast<ChangeHandler::OnChangeFunc>(function);
				
			handler.push_back(changeHandler);
		}
		
		template <typename T>
		void Remove(void *source, T function)
		{
			for (std::list<ClickHandler>::iterator it = handler.begin(); it != handler.end();)
			{
				ChangeHandler &changeHandler = *it;
				if (changeHandler.source == (ChangeHandler*)source && changeHandler.function == reinterpret_cast<ChangeHandler::OnChangeFunc>(function))
				{
					it = handler.erase(it);
				}
				else
				{
					it++;
				}
			}
		}
		
		void Invoke(Control *sender)
		{
			for (std::list<ChangeHandler>::iterator it = handler.begin(); it != handler.end(); it++)
			{
				ChangeHandler &changeHandler = *it;
				(changeHandler.source->*changeHandler.function)(sender);
			}
		}
	};
	
	typedef ChangeEventHandler MouseEnterEventHandler;
	
	typedef ChangeEventHandler MouseLeaveEventHandler;
	
	typedef ChangeEventHandler FocusInEventHandler;
	
	typedef ChangeEventHandler FocusOutEventHandler;
}

#endif