/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "PictureBox.hpp"
#include "../Drawing/TextureAnimator.hpp"
#include "../Misc/Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//static attributes
	//---------------------------------------------------------------------------
	const Drawing::SizeF PictureBox::DefaultSize(100, 100);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PictureBox::PictureBox()
		: stretch(false)
	{
		type = ControlType::PictureBox;

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance()->GetTheme());

		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void PictureBox::SetImage(const Drawing::ImagePtr &_image)
	{
		image = _image;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Drawing::ImagePtr& PictureBox::GetImage()
	{
		return image;
	}
	//---------------------------------------------------------------------------
	void PictureBox::SetStretch(bool _stretch)
	{
		stretch = _stretch;
		if (image)
		{
			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	bool PictureBox::GetStretch() const
	{
		return stretch;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool PictureBox::Intersect(const Drawing::PointF &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	void PictureBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(geometry);

		if (GetBackColor().A > 0)
		{
			g.FillRectangle(GetBackColor(), PointF(0, 0), GetSize());
		}

		if (image)
		{
			if (stretch)
			{
				g.DrawImage(image, Color::White(), RectangleF(PointF(0, 0), GetSize()));
			}
			else
			{
				g.DrawImage(image, Color::White(), PointF(0, 0), RectangleF(PointF(0, 0), GetSize()));
			}
		}
	}
	//---------------------------------------------------------------------------
}
