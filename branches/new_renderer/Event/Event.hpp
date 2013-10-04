/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EVENT_EVENT_HPP
#define OSHGUI_EVENT_EVENT_HPP

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
		class EventHandlerInfo
		{
		public:
			EventHandler<Signature> eventHandler;
			bool remove;
		};

		std::vector<EventHandlerInfo> eventHandlerInfos;

	public:
		/**
		 * Registriert einen EventHandler.
		 *
		 * @param eventHandler
		 */
		Event& operator += (const EventHandler<Signature> &eventHandler)
		{
			EventHandlerInfo info;
			info.eventHandler = eventHandler;
			info.remove = false;
			eventHandlerInfos.push_back(info);

			return *this;
		}
		
		/**
		 * Entfernt einen EventHandler.
		 *
		 * @param eventHandler
		 */
		Event& operator -= (const EventHandler<Signature> &eventHandler)
		{
			for (auto it = std::begin(eventHandlerInfos); it != std::end(eventHandlerInfos); ++it)
			{
				EventHandlerInfo &info = *it;
				if (info.eventHandler == eventHandler)
				{
					info.remove = true;
				}
			}

			return *this;
		}
		
		/**
		 * Ruft alle registrierten EventHandler auf.
		 *
		 * @param param1 Funktionsparameter
		 */
		template <typename T>
		void Invoke(T&& param1)
		{
			if (!eventHandlerInfos.empty())
			{
				for (auto it = std::begin(eventHandlerInfos); it != std::end(eventHandlerInfos);)
				{
					EventHandlerInfo &info = *it;
					if (!info.remove)
					{
						info.eventHandler.GetHandler()(std::forward<T>(param1));
					}
					if (info.remove)
					{
						it = eventHandlerInfos.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}

		/**
		 * Ruft alle registrierten EventHandler auf.
		 *
		 * @param param1 Funktionsparameter
		 * @param param2 Funktionsparameter
		 */
		template <typename T, typename T2>
		void Invoke(T&& param1, T2&& param2)
		{
			if (!eventHandlerInfos.empty())
			{
				for (auto it = std::begin(eventHandlerInfos); it != std::end(eventHandlerInfos);)
				{
					EventHandlerInfo &info = *it;
					if (!info.remove)
					{
						info.eventHandler.GetHandler()(std::forward<T>(param1), std::forward<T2>(param2));
					}
					if (info.remove)
					{
						it = eventHandlerInfos.erase(it);
					}
					else
					{
						++it;
					}
				}
			}
		}
	};
}

#endif