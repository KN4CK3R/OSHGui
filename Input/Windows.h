#ifndef __OSHGUI_INPUT_WINDOWS_H__
#define __OSHGUI_INPUT_WINDOWS_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace OSHGui
{
	namespace Input
	{
		class Windows
		{
		public:
			bool ProcessMessage(LPMSG message);
		};
	}
}

#endif