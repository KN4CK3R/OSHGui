/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_CONCURRENDBOUNDEDBUFFER_HPP
#define OSHGUI_MISC_CONCURRENDBOUNDEDBUFFER_HPP

#include <array>
#include <mutex>
#include <condition_variable>

namespace OSHGui
{
	namespace Misc
	{
		template<typename T, size_t Size>
		class BoundedBuffer
		{
		public:
			typedef size_t size_type;
			typedef T& reference;
			typedef const T& const_reference;

			BoundedBuffer()
				: front(0),
				  rear(0),
				  count(0)
			{

			}

			void Push(const_reference v)
			{
				std::unique_lock<std::mutex> lock(mutex);

				not_full.wait(lock, [this]() { return count != Size; });

				buffer[rear] = v;
				rear = (rear + 1) % Size;
				++count;
				not_empty.notify_one();
			}

			reference Get()
			{
				std::unique_lock<std::mutex> lock(mutex);

				not_empty.wait(lock, [this]() { return count != 0; });

				reference result = buffer[front];
				front = (front + 1) % Size;
				--count;

				not_full.notify_one();

				return result;
			}

			const_reference Get() const
			{
				std::unique_lock<std::mutex> lock(mutex);

				not_empty.wait(lock, [this]() { return count != 0; });

				const_reference result = buffer[front];
				front = (front + 1) % Size;
				--count;

				not_full.notify_one();

				return result;
			}

			size_type size() const
			{
				return count;
			}

			bool empty() const
			{
				return count == 0;
			}

		private:
			int front;
			int rear;
			int count;
			std::array<T, Size> buffer;

			std::mutex mutex;
			std::condition_variable not_full;
			std::condition_variable not_empty;
		};
	}
}

#endif