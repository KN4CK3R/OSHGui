#ifndef __OSHGUI_MISC_STRINGBUFFER_H__
#define __OSHGUI_MISC_STRINGBUFFER_H__

#include <windows.h>

namespace OSHGui
{
	namespace Misc
	{
		class StringBuffer
		{
		public:
			StringBuffer(int size = 1);
			~StringBuffer();
			
			const char& operator[](int index) const;
			char& operator[](int index);
			int GetLength();
			const char* GetBuffer();
			
			void Clear();
			bool InsertChar(int index, char wc);
			bool RemoveChar(int index = -1);
			bool InsertString(int index, const char* str, int count = -1);
			bool SetText(LPCSTR text);
			
			int CaretToPosition(int caret);
			int PositionToCaret(int position);
			
		protected:
			bool SetBufferSize(int size);
		
			char *buffer;
			int size;
		};
	}
}

#endif