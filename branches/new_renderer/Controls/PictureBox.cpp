/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
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
	const Drawing::SizeI PictureBox::DefaultSize(100, 100);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PictureBox::PictureBox()
		: stretch(false)
	{
		type = ControlType::PictureBox;

		SetSize(DefaultSize);
		
		ApplyTheme(Application::Instance().GetTheme());

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
	void PictureBox::SetStretch(bool stretch_)
	{
		stretch = stretch_;
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
	void PictureBox::PopulateGeometry()
	{
		using namespace Drawing;

		Graphics g(*geometry);

		if (GetBackColor().GetAlpha() > 0)
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
