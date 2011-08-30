#ifndef __OSHGUI_MISC_ANY_H__
#define __OSHGUI_MISC_ANY_H__

namespace OSHGui
{
	namespace Misc
	{
		class Any
		{
		private:
			/**
			 * Verhindert "ISO C++ forbids declaration of 'TypeWrapper' with no type"
			 */
			class AnyTypeWrapper
			{
			public:
				virtual ~AnyTypeWrapper() { }
				virtual AnyTypeWrapper* Copy() = 0;
				virtual void* GetObject() = 0;
			};

			template<class T> class TypeWrapper : public AnyTypeWrapper
			{
			private:
				T obj;
				
			public:
				TypeWrapper(const T &object) : obj(object) { }

				TypeWrapper* Copy()
				{
					return new TypeWrapper<T>(obj);
				}
				
				void* GetObject()
				{
					return &obj;
				}
			};
		
			unsigned int id;
			AnyTypeWrapper *wrapper;
			
			static unsigned int NextID()
			{
				static unsigned int id = 0;
				++id;	
				return id;
			}

			template<class T> static unsigned int TypeID()
			{
				static unsigned int id = NextID();
				return id;
			}
			
		public:
			Any()
			{
				id = 0;
				wrapper = 0;
			}
			
			template<class T> Any(const T &obj)
			{
				id = TypeID<T>();
				wrapper = new TypeWrapper<T>(obj);
			}

			Any(const Any &any)
			{
				id = any.id;
				wrapper = any.wrapper->Copy();
			}

			Any& operator= (Any const& any)
			{
				if (this != &any)
				{
					id = any.id;
					wrapper = any.wrapper->Copy();
				}
				return *this;
			}

			operator void *() const
			{
				return (wrapper == 0 ? 0 : (void*)this);
			}
			
			~Any()
			{
				delete wrapper;
			}
			
			template<class T> T CastTo()
			{
				if (TypeID<T>() == id)
				{
					return *static_cast<T*>(wrapper->GetObject());
				}
				else
				{
					throw 1;
				}
			}
		};
	}
}

#endif