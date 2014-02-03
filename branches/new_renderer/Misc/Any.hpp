/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_MISC_ANY_HPP
#define OSHGUI_MISC_ANY_HPP

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
			template<class T>
			class TypeWrapper : public AnyTypeWrapper
			{
			private:
				T obj_;
				
			public:
				TypeWrapper(const T &object) : obj_(object) { }
				//---------------------------------------------------------------------------
				TypeWrapper* Copy()
				{
					return new TypeWrapper<T>(obj_);
				}
				//---------------------------------------------------------------------------
				virtual void* GetObject()
				{
					return &obj_;
				}
			};
			//---------------------------------------------------------------------------
			unsigned int id_;
			AnyTypeWrapper *wrapper_;
			//---------------------------------------------------------------------------
			static unsigned int NextID()
			{
				static unsigned int id = 0;
				++id;
				return id;
			}
			//---------------------------------------------------------------------------
			template<class T>
			static unsigned int TypeID()
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
				id_ = 0;
				wrapper_ = nullptr;
			}
			~Any()
			{
				delete wrapper_;
			}

			/**
			 * Erzeugt ein Any-Objekt, das das angegebene Objekt enthält.
			 *
			 * \param obj
			 */
			template<class T>
			Any(const T &obj)
			{
				id_ = TypeID<T>();
				wrapper_ = new TypeWrapper<T>(obj_);
			}
			/**
			 * Kopierkonstruktor
			 */
			Any(const Any &any)
			{
				id_ = any.id_;
				wrapper_ = any.wrapper_->Copy();
			}
			/**
			 * Weißt diesem Any-Objekt das angegebene zu.
			 *
			 * \param any
			 * \return this
			 */
			Any& operator=(Any const& any)
			{
				if (this != &any)
				{
					delete wrapper_;
					wrapper_ = nullptr;
					
					id_ = any.id_;
					if (any.wrapper_ != nullptr)
					{
						wrapper_ = any.wrapper_->Copy();
					}
				}
				return *this;
			}
			/**
			 * Weißt diesem Any-Objekt eine Variable zu.
			 *
			 * \param obj
			 * \return this
			 */
			template<class T>
			Any& operator=(const T &obj)
			{
				delete wrapper_;
			
				id_ = TypeID<T>();
				wrapper_ = new TypeWrapper<T>(obj);
				
				return *this;
			}
			/**
			 * Dieser Operator erlaubt per if (!any) { any ist leer } zu prüfen, ob das Any-Objekt leer ist.
			 */
			operator void *() const
			{
				return id_ == 0 ? nullptr : (void*)1;
			}

			/**
			 * Castet ein Any-Objekt zu dem in ihm befindlichen Datentyp. Falls ein falscher Datentyp angegeben wird,
			 * wird eine Exception ausgelöst.
			 *
			 * \return das aufgenommene Objekt
			 */
			template<class T>
			T CastTo() const
			{
				if (TypeID<T>() == id_)
				{
					return *static_cast<T*>(wrapper_->GetObject());
				}
				else
				{
					#ifndef OSHGUI_DONTUSEEXCEPTIONS
					throw InvalidOperationException("Cannot cast object");
					#endif
					throw 1;
				}
			}
		};
	}
}

#endif