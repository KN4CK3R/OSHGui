#ifndef OSH_VECTOR_HPP
#define OSH_VECTOR_HPP

namespace OSHGui
{
	namespace Drawing
	{
		class Vector
		{
		public:
			Vector();
			Vector(float x, float y, float z);
			Vector(const Vector &v);
			Vector(const float *farray);

			float Length() const;
			float DotProduct(const Vector &dot) const;
			Vector Normalize() const;
			float DegreesBetweenVector(const Vector &to) const;
	
			Vector& operator=(const Vector &vector);
			Vector& operator=(const float *floatArray);
			Vector& operator=(float val);
	
			float operator[](unsigned int element);
			operator float *();
			operator const float *() const;

			const Vector operator-() const;

			friend bool operator==(const Vector &lhs, const Vector &rhs);
			friend bool operator<(const Vector &lhs, const Vector &rhs);

			Vector& operator+=(const Vector& rhs);
			Vector& operator-=(const Vector& rhs);
			Vector& operator*=(float rhs);
			Vector& operator/=(float rhs);

			float x;
			float y;
			float z;
		};

		bool operator==(const Vector &lhs, const Vector &rhs);
		inline bool operator!=(const Vector &lhs, const Vector &rhs) { return !(lhs == rhs); }
		bool operator<(const Vector &lhs, const Vector &rhs);
		inline bool operator>(const Vector &lhs, const Vector &rhs) { return rhs < lhs; }
		inline bool operator>=(const Vector &lhs, const Vector &rhs) { return !(rhs < lhs); }
		inline bool operator<=(const Vector &lhs, const Vector &rhs) { return !(rhs > lhs); }
		const Vector operator+(const Vector &lhs, const Vector &rhs);
		const Vector operator-(const Vector &lhs, const Vector &rhs);
		const Vector operator*(float lhs, const Vector &rhs);
		const Vector operator*(const Vector &lhs, float rhs);
		const float operator*(const Vector &lhs, const Vector &rhs);
		const Vector operator/(const Vector &lhs, float rhs);
	}
}

#endif