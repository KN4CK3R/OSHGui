#ifndef OSHGUI_INPUT_WINDOWS_H_
#define OSHGUI_INPUT_WINDOWS_H_

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace OSHGui
{
	namespace Input
	{
		/**
		 * Verwaltet den Input unter Windows.
		 */
		class Windows
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