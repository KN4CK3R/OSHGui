/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
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
	const Drawing::Size PictureBox::DefaultSize(100, 100);
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PictureBox::PictureBox()
		: stretch(false)
	{
		type = CONTROL_PICTUREBOX;

		SetSize(DefaultSize);
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::Empty());

		isFocusable = false;
	}
	//---------------------------------------------------------------------------
	PictureBox::~PictureBox()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void PictureBox::SetSize(const Drawing::Size &size)
	{
		if (this->size != size)
		{
			Control::SetSize(size);

			if (image != nullptr)
			{
				std::shared_ptr<Drawing::ITexture> newImage = Application::Instance()->GetRenderer()->CreateNewTexture(GetSize());
				newImage->BeginUpdate();
				newImage->Insert(0, 0, image);
				newImage->EndUpdate();

				image = newImage;
			}
		}
	}
	//---------------------------------------------------------------------------
	void PictureBox::SetImage(const std::shared_ptr<Drawing::ITexture> &image)
	{
		if (this->image != nullptr)
		{
			Drawing::TextureAnimator::StopAnimate(this->image);
		}
		
		this->image = image;
		
		if (this->image != nullptr)
		{
			Drawing::TextureAnimator::Animate(this->image, Drawing::TextureAnimator::Loop);
		}
	}
	//---------------------------------------------------------------------------
	std::shared_ptr<Drawing::ITexture> PictureBox::GetImage() const
	{
		return image;
	}
	//---------------------------------------------------------------------------
	void PictureBox::SetStretch(bool stretch)
	{
		this->stretch = stretch;
	}
	//---------------------------------------------------------------------------
	bool PictureBox::GetStretch() const
	{
		return stretch;
	}
	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool PictureBox::Intersect(const Drawing::Point &point) const
	{
		return Intersection::TestRectangle(absoluteLocation, size, point);
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	void PictureBox::Render(Drawing::IRenderer *renderer)
	{
		if (!isVisible)
		{
			return;
		}
	
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(absoluteLocation, size);
		}
		
		if (image != nullptr)
		{
			Drawing::Size renderSize = size;
			if (!stretch && image->GetSize() < size)
			{
				renderSize = image->GetSize();
			}
			renderer->RenderTexture(image, absoluteLocation, renderSize);
		}
	}
	//---------------------------------------------------------------------------
}