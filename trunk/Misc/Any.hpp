#ifndef OSHGUI_MISC_ANY_HPP_
#define OSHGUI_MISC_ANY_HPP_

#include "../Exports.hpp"

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Dieser Datentyp kann jeden anderen Datentyp aufnehmen.
		 */
		class OSHGUI_EXPORT Any
		{
		private:
			//Verhindert "ISO C++ forbids declaration of 'TypeWrapper' with no type"
			class AnyTypeWrapper
			{
			public:
				virtual ~AnyTypeWrapper() { }
				virtual AnyTypeWrapper* Copy() = 0;
				virtual void* GetObject() = 0;
			};
			//---------------------------------------------------------------------------
			template<class T> class TypeWrapper : public AnyTypeWrapper
			{
			private:
				T obj;
				
			public:
				TypeWrapper(const T &object) : obj(object) { }
				//---------------------------------------------------------------------------
				TypeWrapper* Copy()
				{
					return new TypeWrapper<T>(obj);
				}
				//---------------------------------------------------------------------------
				virtual void* GetObject()
				{
					return &obj;
				}
			};
			//---------------------------------------------------------------------------
			unsigned int id;
			AnyTypeWrapper *wrapper;
			//---------------------------------------------------------------------------
			static unsigned int NextID()
			{
				static unsigned int id = 0;
				++id;
				return id;
			}
			//---------------------------------------------------------------------------
			template<class T> static unsigned int TypeID()
			{
				static unsigned int id = NextID();
				return id;
			}
			//---------------------------------------------------------------------------
			
		public:
			/**
			 * Erzeugt ein leeres Any-Objekt.
			 */
			Any()
			{
				id = 0;
				wrapper = 0;
			}
			~Any()
			{
				delete wrapper;
			}

			/**
			 * Erzeugt ein Any-Objekt, das das angegebene Objekt enthält.
			 *
			 * @param obj
			 */
			template<class T>
			Any(const T &obj)
			{
				id = TypeID<T>();
				wrapper = new TypeWrapper<T>(obj);
			}
			/**
			 * Kopierkonstruktor
			 */
			Any(const Any &any)
			{
				id = any.id;
				wrapper = any.wrapper->Copy();
			}
			/**
			 * Weißt diesem Any-Objekt das angegebene zu.
			 *
			 * @param any
			 * @return this
			 */
			Any& operator= (Any const& any)
			{
				if (this != &any)
				{
					delete wrapper;
					wrapper = 0;
					
					id = any.id;
					if (any.wrapper != 0)
					{
						wrapper = any.wrapper->Copy();
					}
				}
				return *this;
			}
			/**
			 * Weißt diesem Any-Objekt eine Variable zu.
			 *
			 * @param obj
			 * @return this
			 */
			template<class T>
			Any& operator= (const T &obj)
			{			     
				delete wrapper;
			     
				id = TypeID<T>();
				wrapper = new TypeWrapper<T>(obj);
				
				return *this;
			}
			/**
			 * Dieser Operator erlaubt per if (!any) { any ist leer } zu prüfen, ob das Any-Objekt leer ist.
			 */
			operator void *() const
			{
				return (id == 0 ? 0 : (void*)1);
			}

			/**
			 * Castet ein Any-Objekt zu dem in ihm befindlichen Datentyp. Falls ein falscher Datentyp angegeben wird,
			 * wird eine Exception ausgelöst.
			 *
			 * @return das aufgenommene Objekt
			 */
			template<class T>
			T CastTo() const
			{
				if (TypeID<T>() == id)
				{
					return *static_cast<T*>(wrapper->GetObject());
				}
				else
				{
					throw InvalidOperationException(L"Cannot cast object", __WFILE__, __LINE__);
				}
			}
		};
	}
}

#endif