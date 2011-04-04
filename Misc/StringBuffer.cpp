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
		const char& StringBuffer::operator[](int index) const
		{
			if (index < 0 || index >= size)
			{
				throw "out of range";
			}
			
			return buffer[index];
		}
		//---------------------------------------------------------------------------
		char& StringBuffer::operator[](int index)
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
		const char* StringBuffer::GetBuffer()
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
		bool StringBuffer::InsertChar(int index, char wc)
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

			char *dest = buffer + length + 1,
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
			
			memmove(buffer + index, buffer + index + 1, sizeof(char) * (length - index));
			
			return true;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::InsertString(int index, const char* str, int count)
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

			memmove(buffer + index + count, buffer + index, sizeof(char) * (length - index + 1));
			memcpy(buffer + index, str, count * sizeof(char));

			return true;
		}
		//---------------------------------------------------------------------------	
		bool StringBuffer::SetText(LPCSTR text)
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
			
			strcpy_s(buffer, required, text);
			
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

			char *tempBuffer = new char[allocSize];
			if (tempBuffer  == NULL)
			{
				return false;
			}
			
			if (buffer != NULL)
			{
				memcpy(tempBuffer, buffer, size * sizeof(char));
				delete[] buffer;
			}
			else
			{
				memset(tempBuffer, 0x00, sizeof(char) * allocSize);
			}

			buffer = tempBuffer;
			size = allocSize;
			
			return true;
		}
		//---------------------------------------------------------------------------
	}
}