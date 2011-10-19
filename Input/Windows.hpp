#ifndef OSHGUI_INPUT_WINDOWS_HPP_
#define OSHGUI_INPUT_WINDOWS_HPP_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "Exports.hpp"

namespace OSHGui
{
	namespace Input
	{
		/**
		 * Verwaltet den Input unter Windows.
		 */
		class OSHGUI_EXPORT Windows
		{
		public:
			/**
			 * Wandelt eine Windows Message in ein Event um.
			 *
			 * @param message
			 * @return
			 */
			bool ProcessMessage(LPMSG message);
		};
	}
}

#endif