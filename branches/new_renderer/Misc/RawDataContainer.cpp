/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
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
		RawDataContainer::RawDataContainer(std::uint32_t size)
			: data(size)
		{

		}
		//---------------------------------------------------------------------------
		std::uint8_t* RawDataContainer::GetDataPointer()
		{
			return data.data();
		}
		//---------------------------------------------------------------------------
		const std::uint8_t* RawDataContainer::GetDataPointer() const
		{
			return data.data();
		}
		//---------------------------------------------------------------------------
		std::uint32_t RawDataContainer::GetSize() const
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
				throw;
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