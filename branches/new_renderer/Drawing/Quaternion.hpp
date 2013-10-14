#ifndef _CEGUIQuaternion_h_
#define _CEGUIQuaternion_h_

#include <cmath>

namespace OSHGui
{
	namespace Drawing
	{
		class OSHGUI_EXPORT Quaternion
		{
		public:
			//! verbatim constructor
			inline Quaternion(float w = 1.0f, float x = 0.0f, float y = 0.0f, float z = 0.0f):
				w(w),
				x(x),
				y(y),
				z(z)
			{
			
			}

			//! copy constructor
			inline Quaternion(const Quaternion &v):
				w(v.w),
				x(v.x),
				y(v.y),
				z(v.z)
			{
			
			}

			//! assignment operator
			inline Quaternion& operator=(const Quaternion &v)
			{
				w = v.w;
				x = v.x;
				y = v.y;
				z = v.z;
				
				return *this;
			}

			//! equality operator
			inline bool operator==(const Quaternion &v) const
			{
				return (w == v.w) && (x == v.x) && (y == v.y) && (z == v.z);
			}

			//! inequality operator
			inline bool operator!=(const Quaternion &v) const
			{
				return (w != v.w) || (x != v.x) || (y != v.y) || (z != v.z);
			}

			//! negation operator
			inline Quaternion operator-() const
			{
				return Quaternion(-w, -x, -y, -z);
			}

			//! scalar multiplication operator
			inline Quaternion operator*(float v) const
			{
				return Quaternion(w * v, x * v, y * v, z * v);
			}

			//! scalar multiplication operator
			inline friend Quaternion operator*(float v, const Quaternion &q)
			{
				return Quaternion(v * q.w, v * q.x, v * q.y, v * q.z);
			}

			//! quaternion dot product
			inline float Dot(const Quaternion &v) const
			{
				return w * v.w + x * v.x + y * v.y + z * v.z;
			}

			//! addition operator
			inline Quaternion operator+(const Quaternion &v) const
			{
				return Quaternion(w + v.w, x + v.x, y + v.y, z + v.z);
			}

			/*!
			\brief quaternion multiplication (not commutative!)

			Lets say we have quaternion A describing a rotation and another quaternion B.
			If we write C = A * B, C is actually describing a rotation we would get if we
			rotated Identity by A and then rotated the result by B
			*/
			inline Quaternion operator*(const Quaternion &v) const
			{
				return Quaternion(
					w * v.w - x * v.x - y * v.y - z * v.z,
					w * v.x + x * v.w + y * v.z - z * v.y,
					w * v.y + y * v.w + z * v.x - x * v.z,
					w * v.z + z * v.w + x * v.y - y * v.x
				);
			}

			/*!
			\brief computers and returns the length of this quaternion
			*/
			inline float Length() const
			{
				return sqrtf((w * w) + (x * x) + (y * y) + (z * z));
			}

			/*!
			\brief normalises this quaternion and returns it's length (since it has to be computed anyways)
			*/
			inline float Normalise()
			{
				const float len = Length();
				const float factor = 1.0f / len;
				*this = *this * factor;

				return len;
			}

			static Quaternion eulerAnglesRadians(const float x, const float y, const float z)
			{
				// the order of rotation:
				// 1) around Z
				// 2) around Y
				// 3) around X
				// even though it's passed in reverse...

				const float sin_z_2 = sinf(0.5f * z);
				const float sin_y_2 = sinf(0.5f * y);
				const float sin_x_2 = sinf(0.5f * x);

				const float cos_z_2 = cosf(0.5f * z);
				const float cos_y_2 = cosf(0.5f * y);
				const float cos_x_2 = cosf(0.5f * x);

				return Quaternion(
					cos_z_2 * cos_y_2 * cos_x_2 + sin_z_2 * sin_y_2 * sin_x_2,
					cos_z_2 * cos_y_2 * sin_x_2 - sin_z_2 * sin_y_2 * cos_x_2,
					cos_z_2 * sin_y_2 * cos_x_2 + sin_z_2 * cos_y_2 * sin_x_2,
					sin_z_2 * cos_y_2 * cos_x_2 - cos_z_2 * sin_y_2 * sin_x_2
					);
			}

			//----------------------------------------------------------------------------//
			static Quaternion eulerAnglesDegrees(const float x, const float y, const float z)
			{
				static const float d2r = (4.0f * std::atan2(1.0f, 1.0f)) / 180.0f;

				return eulerAnglesRadians(x * d2r, y * d2r, z * d2r);
			}

			//! Quaternion(0, 0, 0, 0)
			//static const Quaternion ZERO;
			//! Quaternion(1, 0, 0, 0)
			//static const Quaternion IDENTITY;

			//! imaginary part
			float w;
			//! x component of the vector part
			float x;
			//! y component of the vector part
			float y;
			//! z component of the vector part
			float z;
		};
	}
}

#endif
