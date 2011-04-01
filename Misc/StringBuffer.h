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
			
			const WCHAR& operator[](int index) const;
			WCHAR& operator[](int index);
			int GetLength();
			const WCHAR* GetBuffer();
			
			void Clear();
			bool InsertChar(int index, WCHAR wc);
			bool RemoveChar(int index = -1);
			bool InsertString(int index, const WCHAR* str, int count = -1);
			bool SetText(LPCWSTR text);
			
			int CaretToPosition(int caret);
			int PositionToCaret(int position);
			
		protected:
			bool SetBufferSize(int size);
		
			WCHAR *buffer;
			int size;
		};
	}
}

#endif