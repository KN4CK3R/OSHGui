#include "ColorRectangle.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		ColorRectangle::ColorRectangle()
		{

		}
		//---------------------------------------------------------------------------
		ColorRectangle::ColorRectangle(const Color &col)
			: TopLeft(col),
			  TopRight(col),
			  BottomLeft(col),
			  BottomRight(col)
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
		void ColorRectangle::SetAlpha(std::uint8_t alpha)
		{
			TopLeft.A = alpha;
			TopRight.A = alpha;
			BottomLeft.A = alpha;
			BottomRight.A = alpha;
		}
		//---------------------------------------------------------------------------
		void ColorRectangle::SetTopAlpha(std::uint8_t alpha)
		{
			TopLeft.A = alpha;
			TopRight.A = alpha;
		}
		//---------------------------------------------------------------------------
		void ColorRectangle::SetBottomAlpha(std::uint8_t alpha)
		{
			BottomLeft.A = alpha;
			BottomRight.A = alpha;
		}
		//---------------------------------------------------------------------------
		void ColorRectangle::SetLeftAlpha(std::uint8_t alpha)
		{
			TopRight.A = alpha;
			BottomLeft.A = alpha;
		}
		//---------------------------------------------------------------------------
		void ColorRectangle::SetRightAlpha(std::uint8_t alpha)
		{
			TopRight.A = alpha;
			BottomRight.A = alpha;
		}
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
		void ColorRectangle::SetColors(const Color &col)
		{
			TopLeft = TopRight = BottomLeft = BottomRight = col;
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
