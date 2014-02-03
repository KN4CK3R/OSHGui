/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
			EventHandler<Signature> EventHandler;
			bool Remove;
		};

		std::vector<EventHandlerInfo> eventHandlerInfos_;

	public:
		/**
		 * Registriert einen EventHandler.
		 *
		 * \param eventHandler
		 */
		Event& operator += (const EventHandler<Signature> &eventHandler)
		{
			EventHandlerInfo info = {
				eventHandler,
				false
			};
			eventHandlerInfos_.push_back(info);

			return *this;
		}
		
		/**
		 * Entfernt einen EventHandler.
		 *
		 * \param eventHandler
		 */
		Event& operator -= (const EventHandler<Signature> &eventHandler)
		{
			for (auto &info : eventHandlerInfos_)
			{
				if (info.EventHandler == eventHandler)
				{
					info.Remove = true;
				}
			}

			return *this;
		}
		
		/**
		 * Ruft alle registrierten EventHandler auf.
		 *
		 * \param param1 Funktionsparameter
		 */
		template <typename T>
		void Invoke(T&& param1)
		{
			if (!eventHandlerInfos_.empty())
			{
				for (auto it = std::begin(eventHandlerInfos_); it != std::end(eventHandlerInfos_);)
				{
					auto &info = *it;
					if (!info.Remove)
					{
						info.EventHandler.GetHandler()(std::forward<T>(param1));
					}
					if (info.Remove)
					{
						it = eventHandlerInfos_.erase(it);
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
		 * \param param1 Funktionsparameter
		 * \param param2 Funktionsparameter
		 */
		template <typename T, typename T2>
		void Invoke(T&& param1, T2&& param2)
		{
			if (!eventHandlerInfos_.empty())
			{
				for (auto it = std::begin(eventHandlerInfos_); it != std::end(eventHandlerInfos_);)
				{
					auto &info = *it;
					if (!info.Remove)
					{
						info.EventHandler.GetHandler()(std::forward<T>(param1), std::forward<T2>(param2));
					}
					if (info.Remove)
					{
						it = eventHandlerInfos_.erase(it);
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