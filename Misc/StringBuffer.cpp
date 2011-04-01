#include "StringBuffer.h"

namespace OSHGui
{
	namespace Misc
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		StringBuffer::StringBuffer(int size)
		{
			this->size = size < 1 ? 1 : size;
			
			buffer = NULL;
			
			SetBufferSize(this->size);
		}
		//---------------------------------------------------------------------------
		StringBuffer::~StringBuffer()
		{
			delete[] buffer;
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const WCHAR& StringBuffer::operator[](int index) const
		{
			if (index < 0 || index >= size)
			{
				throw "out of range";
			}
			
			return buffer[index];
		}
		//---------------------------------------------------------------------------
		WCHAR& StringBuffer::operator[](int index)
		{
			if (index < 0 || index >= size)
			{
				throw "out of range";
			}
			
			return buffer[index];
		}
		//---------------------------------------------------------------------------
		int StringBuffer::GetLength()
		{
			return lstrlenW(buffer);
		}
		//---------------------------------------------------------------------------
		const WCHAR* StringBuffer::GetBuffer()
		{
			return buffer;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------	
		void StringBuffer::Clear()
		{
			*buffer = 0;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::InsertChar(int index, WCHAR wc)
		{
			int length = GetLength();
			if (index < -1 || index > length)
			{
				return false;
			}

			if (length + 1 >= 1024)
			{
				return false;
			}

			if (length + 1 >= size)
			{
				if (!SetBufferSize(length * 2))
				{
					return false;
				}
			}
			
			if (index == -1)
			{
				index = length;
			}

			WCHAR *dest = buffer + length + 1,
				  *stop = buffer + index,
				  *src = dest - 1;

			while (dest > stop)
			{
				*dest-- = *src--;
			}

			buffer[index] = wc;

			return true;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::RemoveChar(int index)
		{
			int length = GetLength();
			if (length == 0 || index < -1 || index >= length)
			{
				return false;
			}

			if (index == -1)
			{
				index = length;
			}
			
			memmove(buffer + index, buffer + index + 1, sizeof(WCHAR) * (length - index));
			
			return true;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::InsertString(int index, const WCHAR* str, int count)
		{
			int length = GetLength();
			if (length == 0 || index < -1 || str == NULL)
			{
				return false;
			}
			
			count = count == -1 ? lstrlenW(str) : count > lstrlenW(str) ? lstrlenW(str) : count;
			
			if (size + count >= 1024)
			{
				return false;
			}

			if (length + count >= size)
			{
				if (!SetBufferSize(length + count + 1))
				{
					return false;
				}
			}
			
			if (index == -1)
			{
				index = length;
			}

			memmove(buffer + index + count, buffer + index, sizeof(WCHAR) * (length - index + 1));
			memcpy(buffer + index, str, count * sizeof(WCHAR));

			return true;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::SetText(LPCWSTR text)
		{
			int required = lstrlenW(text) + 1;

			if (required >= 1024)
			{
				return false;
			}

			if (size < required)
			{
				if (!SetBufferSize(required))
				{
					return false;
				}
			}
			
			wcscpy_s(buffer, required, text);
			
			return true;
		}
		//---------------------------------------------------------------------------	
		int StringBuffer::CaretToPosition(int caret)
		{
			return -1;
		}
		//---------------------------------------------------------------------------	
		int StringBuffer::PositionToCaret(int position)
		{
			return -1;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::SetBufferSize(int newSize)
		{
			if (newSize > 1023)
			{
				return false;
			}

			int allocSize = (newSize < size * 2) ? size * 2 : 128;
			if (allocSize > 1024)
			{
				allocSize = 1024;
			}

			WCHAR *tempBuffer = new WCHAR[allocSize];
			if (tempBuffer  == NULL)
			{
				return false;
			}
			
			if (buffer != NULL)
			{
				memcpy(tempBuffer, buffer, size * sizeof(WCHAR));
				delete[] buffer;
			}
			else
			{
				memset(tempBuffer, 0x00, sizeof(WCHAR) * allocSize);
			}

			buffer = tempBuffer;
			size = allocSize;
			
			return true;
		}
		//---------------------------------------------------------------------------
	}
}