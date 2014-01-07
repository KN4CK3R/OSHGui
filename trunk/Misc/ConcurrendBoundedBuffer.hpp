/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_CONCURRENDQUEUE_HPP
#define OSHGUI_MISC_CONCURRENDQUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>

namespace OSHGui
{
	namespace Misc
	{
		template<typename T>
		class ConcurrendQueue
		{
		public:
			void Push(const T &item)
			{
				std::unique_lock<std::mutex> lock(mutex);
				
				queue.push(item);
				lock.unlock();
				condition.notify_one();
			}

			void Push(T&& item)
			{
				std::unique_lock<std::mutex> lock(mutex);
				
				queue.push(std::move(item));
				lock.unlock();
				condition.notify_one();
			}

			T Pop()
			{
				std::unique_lock<std::mutex> lock(mutex);
				
				while (queue.empty())
				{
					condition.wait(lock);
				}
				
				auto item = queue.front();
				queue.pop();
				return item;
			}

			void Pop(T &item)
			{
				std::unique_lock<std::mutex> lock(mutex);
				while (queue.empty())
				{
					condition.wait(mlock);
				}
				item = queue_.front();
				queue.pop();
			}

			bool IsEmpty() const
			{
				return queue.empty();
			}

		private:
			std::queue<T> queue;
			std::mutex mutex;
			std::condition_variable condition;
		};
	}
}

#endif