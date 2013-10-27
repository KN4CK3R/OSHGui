/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_RAWDATACONTAINER_HPP
#define OSHGUI_MISC_RAWDATACONTAINER_HPP

#include <vector>
#include <cstdint>
#include "../Exports.hpp"
#include "Strings.hpp"

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Verwaltet eine Zeichenkette und deren Ausmaﬂe.
		 */
		class OSHGUI_EXPORT RawDataContainer
		{
		public:
			/**
			 * Erstellt ein TextHelper-Objekt mit der entsprechenden Schriftart.
			 *
			 * \param font
			 */
			RawDataContainer();

			RawDataContainer(uint32_t size);
			
			uint8_t* GetDataPointer();
			const uint8_t* GetDataPointer() const;
			uint32_t GetSize() const;

			void LoadFromFile(const Misc::AnsiString &filename);

			void Clear();
			
		private:
			std::vector<uint8_t> data;
		};
	}
}

#endif