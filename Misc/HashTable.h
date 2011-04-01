#ifndef __OSHGUI_MISC_HASHTABLE_H__
#define __OSHGUI_MISC_HASHTABLE_H__

namespace OSHGui
{
	namespace Misc
	{
		template <typename T, typename N> class HashTable
		{
		private:
			template <typename T, typename N> class Pair
			{
			public:
				unsigned long Hash;
				T Key;
				N Data;
			};
			//---------------------------------------------------------------------------
			template <typename T, typename N> class Bucket
			{
			public:
				int Count;
				Pair<T, N> *Pairs;
			};
			//---------------------------------------------------------------------------

		public:
			//---------------------------------------------------------------------------
			//Constructor
			//---------------------------------------------------------------------------
			HashTable(int capacity = 16)
			{		
				this->capacity = capacity < 1 ? 16 : capacity;
				size = 0;
				buckets = NULL;
			}
			//---------------------------------------------------------------------------
			HashTable(const HashTable<T, N> &hashTable)
			{
				capacity = hashTable.capacity;
				size = 0;

				for (int i = 0; i < hashTable.capacity; i++)
				{
					Bucket<T, N> *bucket = &hashTable.buckets[i];
					for (int j = 0; j < bucket->Count; j++)
					{
						Pair<T, N> *pair = &bucket->Pairs[j];
						Add(pair->Key, pair->Data);
					}
				}
			}
			//---------------------------------------------------------------------------
			~HashTable()
			{
				Clear();
			}
			//---------------------------------------------------------------------------
			//---------------------------------------------------------------------------
			//Getter/Setter
			//---------------------------------------------------------------------------
			int GetSize()
			{
				return size;
			}
			//---------------------------------------------------------------------------
			const N& operator[](T key) const
			{
				return Get(key);
			}
			//---------------------------------------------------------------------------
			N& operator[](T key)
			{
				return Get(key);
			}
			//---------------------------------------------------------------------------
			N& Get(T key)
			{
				unsigned long hash = ComputeHash(key);
				Bucket<T, N> *bucket = &buckets[hash % capacity];
				Pair<T, N> *pair = GetPair(bucket, hash);
				if (pair != NULL)
				{
					return pair->Data;
				}
				throw "error";
			}
			//---------------------------------------------------------------------------
			//Runtime-Functions
			//---------------------------------------------------------------------------
			void Init()
			{
				buckets = (Bucket<T, N>*)malloc(capacity * sizeof(Bucket<T, N>));
				if (buckets == NULL)
				{
					throw "error";
				}
				memset(buckets, 0x00, capacity * sizeof(Bucket<T, N>));
			}
			//---------------------------------------------------------------------------
			bool Add(T key, N& data)
			{
				if (buckets == NULL)
				{
					Init();
				}
			
				if (Contains(key))
				{
					return false;
				}
			
				unsigned long hash = ComputeHash(key);
				Bucket<T, N> *bucket = &buckets[hash % capacity];

				if (bucket->Count == 0)
				{
					bucket->Pairs = (Pair<T, N>*)malloc(sizeof(Pair<T, N>));
					if (bucket->Pairs == NULL)
					{
						return false;
					}
					bucket->Count = 1;
				}
				else
				{
					Pair<T, N> *temp = (Pair<T, N>*)realloc(bucket->Pairs, (bucket->Count + 1) * sizeof(Pair<T, N>));
					if (temp == NULL)
					{
						return false;
					}
					bucket->Pairs = temp;
					bucket->Count++;
				}

				Pair<T, N> *pair = &bucket->Pairs[bucket->Count - 1];
				pair->Key = key;
				pair->Hash = hash;
				pair->Data = data;

				size++;

				return true;
			}
			//---------------------------------------------------------------------------
			void Remove(T key)
			{
				if (key == NULL)
				{
					return;
				}
					
				unsigned long hash = ComputeHash(key);
				Bucket<T, N> *bucket = &buckets[hash % capacity];

				if (bucket->Count == 0)
				{
					return;
				}
			
				Pair<T, N> *pair = bucket->Pairs;
			
				int index = 0;
				for (; index < bucket->Count; index++, pair++)
				{
					if (pair->Hash == hash)
					{
						break;
					}
				}

				if (index + 1 < bucket->Count)
				{
					bucket->Pairs[index].~Pair();

					memmove(&bucket->Pairs[index], &bucket->Pairs[index + 1], sizeof(Pair<T, N>) * (bucket->Count - (index + 1)));
				}

				bucket->Count--;
				size--;
			}
			//---------------------------------------------------------------------------
			void Clear()
			{
				if (buckets == NULL)
				{
					return;
				}

				for (int i = 0; i < capacity; i++)
				{
					Bucket<T, N> *bucket = &buckets[i];
					if (bucket->Pairs != NULL)
					{
						free(bucket->Pairs);
						bucket->Count = 0;
					}
				}

				free(buckets);

				capacity = 16;
				size = 0;
				buckets = NULL;
			}
			//---------------------------------------------------------------------------
			bool Contains(T key)
			{
				unsigned long hash = ComputeHash(key);
				Bucket<T, N> *bucket = &buckets[hash % capacity];
				return GetPair(bucket, hash) != NULL;
			}
			//---------------------------------------------------------------------------
			
		private:
			Pair<T, N> *GetPair(Bucket<T, N> *bucket, unsigned long hash)
			{
				if (bucket == NULL || bucket->Count == 0)
				{
					return NULL;
				}

				Pair<T, N> *pair = bucket->Pairs;

				for (int i = 0; i < bucket->Count; i++, pair++)
				{
					if (pair->Hash == hash)
					{
						return pair;
					}
				}

				return NULL;
			}
			//---------------------------------------------------------------------------
			unsigned long ComputeHash(T key)
			{
				/*dan bernstein algorithm*/
				unsigned long hash = 5381;
			
				char *temp = (char*)&key;
			
				for (int i = 0, len = sizeof(T); i < len; i++)
				{
					hash = ((hash << 5) + hash) + (int)temp[i];
				}
			
				return hash;
			}
			//---------------------------------------------------------------------------
		
			int capacity,
				size;
			Bucket<T, N> *buckets;
		};
	}
}

#endif