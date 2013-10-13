#include "Image.hpp"

#include <algorithm>

namespace OSHGui
{
	namespace Drawing
	{
		Image::~Image()
		{

		}
		//---------------------------------------------------------------------------
		void Image::ComputeScalingFactors(AutoScaleMode mode, const SizeF &display, const SizeF &native, float &xScale, float &yScale)
		{
			switch (mode)
			{
				case AutoScaleMode::Disabled:
					xScale = 1.0f;
					yScale = 1.0f;
					break;
				case AutoScaleMode::Vertical:
					xScale = display.Height / native.Height;
					yScale = xScale;
					break;
				case AutoScaleMode::Horizontal:
					xScale = display.Width / native.Width;
					yScale = xScale;
					break;
				case AutoScaleMode::Min:
					xScale = std::min(display.Width / native.Width, display.Height / native.Height);
					yScale = xScale;
					break;
				case AutoScaleMode::Max:
					xScale = std::max(display.Width / native.Width, display.Height / native.Height);
					yScale = xScale;
					break;
				case AutoScaleMode::Both:
					xScale = display.Width / native.Width;
					yScale = display.Height / native.Height;
					break;
				default:
					throw;
			}
			//---------------------------------------------------------------------------
		}
	}
}
