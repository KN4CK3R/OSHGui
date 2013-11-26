/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "RawDataContainer.hpp"
#include "Exceptions.hpp"
#include <fstream>
#include <iterator>

namespace OSHGui
{
	namespace Misc
	{
		RawDataContainer::RawDataContainer()
		{

		}
		//---------------------------------------------------------------------------
		RawDataContainer::RawDataContainer(uint32_t size)
			: data(size)
		{

		}
		//---------------------------------------------------------------------------
		uint8_t* RawDataContainer::GetDataPointer()
		{
			return data.data();
		}
		//---------------------------------------------------------------------------
		const uint8_t* RawDataContainer::GetDataPointer() const
		{
			return data.data();
		}
		//---------------------------------------------------------------------------
		uint32_t RawDataContainer::GetSize() const
		{
			return data.size();
		}
		//---------------------------------------------------------------------------
		void RawDataContainer::LoadFromFile(const Misc::AnsiString &filename)
		{
			Clear();

			std::ifstream in(filename, std::ios::binary);
			if (in)
			{
				std::copy(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>(), std::back_inserter(data));
			}
			else
			{
				throw Misc::FileNotFoundException();
			}
		}
		//---------------------------------------------------------------------------
		void RawDataContainer::Clear()
		{
			data.clear();
		}
		//---------------------------------------------------------------------------
	}
}