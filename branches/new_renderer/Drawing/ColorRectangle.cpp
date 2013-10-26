#include "ColorRectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ColorRectangle::ColorRectangle(const Color &col)
			: TopLeft(col),
			  TopRight(col),
			  BottomLeft(col),
			  BottomRight(col)
		{

		}
		//---------------------------------------------------------------------------
		ColorRectangle::ColorRectangle(const Color &top, const Color &bottom)
			: TopLeft(top),
			  TopRight(top),
			  BottomLeft(bottom),
			  BottomRight(bottom)
		{

		}
		//---------------------------------------------------------------------------
		ColorRectangle::ColorRectangle(const Color &topLeft, const Color &topRight, const Color &bottomLeft, const Color &bottomRight)
			: TopLeft(topLeft),
			  TopRight(topRight),
			  BottomLeft(bottomLeft),
			  BottomRight(bottomRight)
		{

		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		bool ColorRectangle::IsMonochromatic() const
		{
			return TopLeft == TopRight && TopLeft == BottomLeft && TopLeft == BottomRight;
		}
		//---------------------------------------------------------------------------
		Color ColorRectangle::GetColorAtPoint(float x, float y) const
		{
			auto h1((TopRight - TopLeft) * x + TopLeft);
			auto h2((BottomRight - BottomLeft) * x + BottomLeft);
			return (h2 - h1) * y + h1;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		ColorRectangle ColorRectangle::GetSubRectangle(float left, float right, float top, float bottom) const
		{
			return ColorRectangle(
				GetColorAtPoint(left, top),
				GetColorAtPoint(right, top),
				GetColorAtPoint(left, bottom),
				GetColorAtPoint(right, bottom)
			);
		}
		//---------------------------------------------------------------------------
		ColorRectangle& ColorRectangle::operator+=(const ColorRectangle &rhs)
		{
			TopLeft += rhs.TopLeft;
			TopRight += rhs.TopRight;
			BottomLeft += rhs.BottomLeft;
			BottomRight += rhs.BottomRight;

			return *this;
		}
		//---------------------------------------------------------------------------
		ColorRectangle& ColorRectangle::operator*=(const ColorRectangle &rhs)
		{
			TopLeft *= rhs.TopLeft;
			TopRight *= rhs.TopRight;
			BottomLeft *= rhs.BottomLeft;
			BottomRight *= rhs.BottomRight;

			return *this;
		}
		//---------------------------------------------------------------------------
		ColorRectangle& ColorRectangle::operator*=(float rhs)
		{
			TopLeft *= rhs;
			TopRight *= rhs;
			BottomLeft *= rhs;
			BottomRight *= rhs;

			return *this;
		}
		//---------------------------------------------------------------------------
		const ColorRectangle operator+(const ColorRectangle &lhs, const ColorRectangle &rhs)
		{
			auto temp(lhs);
			temp += rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
		const ColorRectangle operator*(const ColorRectangle &lhs, const ColorRectangle &rhs)
		{
			auto temp(lhs);
			temp *= rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
		const ColorRectangle operator*(const ColorRectangle &lhs, float rhs)
		{
			auto temp(lhs);
			temp *= rhs;
			return temp;
		}
		//---------------------------------------------------------------------------
	}
}
