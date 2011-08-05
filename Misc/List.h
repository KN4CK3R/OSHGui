#ifndef __OSHGUI_MISC_LIST_H__
#define __OSHGUI_MISC_LIST_H__

#include <stdlib.h>

/*#ifndef _NEW_
inline void *__cdecl operator new(size_t, void *_Where) throw()
{
	return (_Where);
}
#endif*/

namespace OSHGui
{
	namespace Misc
	{
		template <typename T> class List
		{
		public:
			List()
			{
				data = NULL;
				size = 0;
				maxSize = 0;
			}
			//---------------------------------------------------------------------------
			List(const List<T> &list)
			{
				for (int i = 0; i < list.size; i++)
				{
					Add(list.data[i]);
				}
			}
			//---------------------------------------------------------------------------
			~List()
			{
				Clear();
			}
			//---------------------------------------------------------------------------
			const T& operator[](int index) const
			{
				return Get(index);
			}
			//---------------------------------------------------------------------------
			T& operator[](int index)
			{
				return Get(index);
			}
			//---------------------------------------------------------------------------
			List& operator=(const List<T>& list)
			{
				if (this == &list)
				{
					return *this;
				}

				RemoveAll();
				for (int i = 0; i < list.size; i++)
				{
					Add(list.data[i]);
				}
				return *this;
			}
			//---------------------------------------------------------------------------
			bool Add(const T& value)
			{
				if (!SetSizeInternal(size + 1))
				{
					return false;
				}

				//::new (&data[size]) T;

				data[size++] = value;

				return true;
			}
			//---------------------------------------------------------------------------
			bool Insert(int index, const T& value)
			{
				if (index < 0 || index > size)
				{
					return false;
				}

				if (!SetSizeInternal(size + 1))
				{
					return false;
				}

				memmove(&data[index + 1], &data[index], sizeof(T) * (size - index));

				//::new (&data[index]) T;

				data[index] = value;
				++size;

				return true;
			}
			//---------------------------------------------------------------------------
			bool Set(int index, const T& value)
			{
				if (index < 0 || ndex >= size)
				{
					return false;
				}

				data[index] = value;
				return true;
			}
			//---------------------------------------------------------------------------
			T& Get(int index) const
			{
				if (index >= 0 && index < size)
				{
					return data[index];
				}
				throw "error";
			}
			//---------------------------------------------------------------------------
			int GetSize() const
			{
				return size;
			}
			//---------------------------------------------------------------------------
			bool IsEmpty() const
			{
				return GetSize() == 0;
			}
			//---------------------------------------------------------------------------
			T* GetData()
			{
				return data;
			}
			//---------------------------------------------------------------------------
			bool Remove(int index)
			{
				if (index < 0 || index >= size)
				{
					return false;
				}

				data[index].~T();

				memmove(&data[index], &data[index + 1], sizeof(T) * (size - (index + 1)));
				--size;

				return true;
			}
			//---------------------------------------------------------------------------
			void Clear()
			{
				SetSize(0);
			}
			//---------------------------------------------------------------------------
			bool SetSize(int newMaxSize)
			{
				int oldSize = size;

				if (oldSize > newMaxSize)
				{
					if (data)
					{
						for (int i = newMaxSize; i < oldSize; i++)
						{
							data[i].~T();
						}
					}
				}

				SetSizeInternal(newMaxSize);

				if (oldSize < newMaxSize)
				{
					if (data)
					{
						for (int i = oldSize; i < newMaxSize; i++)
						{
							//::new (&data[i]) T;
						}
					}
				}

				return true;
			}
			//---------------------------------------------------------------------------
			bool Contains(const T& value)
			{
				return (IndexOf(value) != -1);
			}
			//---------------------------------------------------------------------------
			int IndexOf(const T& value)
			{
				return (size > 0) ? IndexOf(value, 0, size) : -1;
			}
			//---------------------------------------------------------------------------
			int IndexOf(const T& value, int start)
			{
				return IndexOf(value, start, size - start);
			}
			//---------------------------------------------------------------------------
			int IndexOf(const T& value, int start, int elements)
			{
				if (start < 0 || start >= size || elements < 0 || start + elements > size)
				{
					return -1;
				}

				for (int i = start, temp = start + elements; i < temp; i++)
				{
					if (value == data[i])
					{
						return i;
					}
				}

				return -1;
			}
			//---------------------------------------------------------------------------
			int LastIndexOf(const T& value)
			{
				return (size > 0) ? LastIndexOf(value, size - 1, size) : -1;
			}
			//---------------------------------------------------------------------------
			int LastIndexOf(const T& value, int index)
			{
				return LastIndexOf(value, index, index + 1);
			}
			//---------------------------------------------------------------------------
			int LastIndexOf(const T& value, int index, int elements)
			{
				if (end < 0 || end >= size || elements < 0 || end - elements < 0)
				{
					return -1;
				}

				for (int i = end, temp = end - elements; i > temp; i--)
				{
					if (value == data[i])
					{
						return i;
					}
				}

				return -1;
			}
			
		protected:
			T *data;
			int size;
			int maxSize;
			//---------------------------------------------------------------------------
			bool SetSizeInternal(int newMaxSize)
			{
				if (newMaxSize < 0 || (newMaxSize > 0xFFFFFFFF / sizeof(T)))
				{
					return false;
				}

				if (newMaxSize == 0)
				{
					if (data)
					{
						free(data);
						data = NULL;
					}

					maxSize = 0;
					size = 0;
				}
				else if (data == NULL || newMaxSize > maxSize)
				{
					int grow = (maxSize == 0) ? 16 : maxSize;

					if ((unsigned int)maxSize + (unsigned int)grow > (unsigned int)0xFFFFFFFF)
					{
						grow = 0xFFFFFFFF - maxSize;
					}

					newMaxSize = newMaxSize > maxSize + grow ? newMaxSize : maxSize + grow;

					if (sizeof(T) > (unsigned int)0xFFFFFFFF / (unsigned int)newMaxSize)
					{
						return false;
					}

					T* newData = (T*)realloc(data, newMaxSize * sizeof(T));
					if (newData == NULL)
					{
						return false;
					}

					data = newData;
					maxSize = newMaxSize;
				}

				return true;
			}
		};
	}
}

#endif