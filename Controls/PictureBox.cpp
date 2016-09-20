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
		: stretch_(false)
	{
		type_ = ControlType::PictureBox;

		SetSize(DefaultSize);
		
		ApplyStyle(Application::Instance().GetStyle());

		isFocusable_ = false;
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void PictureBox::SetImage(const Drawing::ImagePtr &image)
	{
		image_ = image;

		Invalidate();
	}
	//---------------------------------------------------------------------------
	Drawing::ImagePtr& PictureBox::GetImage()
	{
		return image_;
	}
	//---------------------------------------------------------------------------
	void PictureBox::SetStretch(bool stretch)
	{
		stretch_ = stretch;
		if (image_)
		{
			Invalidate();
		}
	}
	//---------------------------------------------------------------------------
	bool PictureBox::GetStretch() const
	{
		return stretch_;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	void PictureBox::PopulateGeometry(Skins::Base &skin)
	{
		skin.DrawPictureBox(Drawing::Graphics(*geometry_), this, image_, stretch_);
	}
	//---------------------------------------------------------------------------
}
