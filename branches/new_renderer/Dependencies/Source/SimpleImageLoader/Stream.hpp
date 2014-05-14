#ifndef SIMPLEIMAGELOADER_STREAM_HPP
#define SIMPLEIMAGELOADER_STREAM_HPP

#include "SimpleImageLoader.hpp"

namespace SimpleImageLoader
{
	class Stream
	{
	public:
		enum class SeekPosition
		{
			Current,
			Begin,
			End
		};

		Stream(const RawData &data);

		void Seek(int32_t offset, SeekPosition position);

		template<typename T>
		inline void Read(T *dst, uint32_t count)
		{
			auto size = sizeof(T)* count;
			if (offset_ + size > data_.size())
			{
				throw;
			}

			std::memcpy(dst, data_.data() + offset_, size);

			offset_ += size;
		}

		uint32_t Read(void *dst, uint32_t size, uint32_t count);

	private:
		const RawData &data_;
		uint32_t offset_;
	};
}

#endif
