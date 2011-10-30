#include "PictureBox.hpp"
#include "..\Drawing\TextureAnimator.hpp"
#include "..\Misc\Exceptions.hpp"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	PictureBox::PictureBox(Control *parent) : Control(parent)
	{
		type = CONTROL_PICTUREBOX;

		SetBounds(6, 6, 100, 100);
		
		SetBackColor(Drawing::Color::Empty());
		SetForeColor(Drawing::Color::Empty());
	}
	//---------------------------------------------------------------------------
	PictureBox::~PictureBox()
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	void PictureBox::SetImage(const std::shared_ptr<Drawing::ITexture> &image)
	{
		if (this->image != 0)
		{
			Drawing::TextureAnimator::StopAnimate(this->image);
		}
		
		this->image = image;
		
		if (this->image != 0)
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
	//Runtime-Functions
	//---------------------------------------------------------------------------
	bool PictureBox::CanHaveFocus() const
	{
		return visible && enabled;
	}
	//---------------------------------------------------------------------------
	bool PictureBox::ContainsPoint(const Drawing::Point &point) const
	{
		return bounds.Contains(point);
	}
	//---------------------------------------------------------------------------
	void PictureBox::Invalidate()
	{
		if (clientArea != bounds)
		{
			clientArea = bounds;

			if (image != 0)
			{
				std::shared_ptr<Drawing::ITexture> newImage = Application::GetRenderer()->CreateNewTexture(GetSize());
				newImage->BeginUpdate();
				newImage->Insert(0, 0, image);
				newImage->EndUpdate();

				image = newImage;
			}
		}

		InvalidateChildren();
	}
	//---------------------------------------------------------------------------
	//Event-Handling
	//---------------------------------------------------------------------------
	IEvent::NextEventTypes PictureBox::ProcessEvent(IEvent *event)
	{
		if (event == 0)
		{
			throw Misc::ArgumentNullException(L"event", __WFILE__, __LINE__);
		}

		if (!visible || !enabled)
		{
			return IEvent::Continue;
		}

		Drawing::Point mousePositionBackup;
		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;
			mousePositionBackup = mouse->Position;
			mouse->Position = PointToClient(mouse->Position);
		}
	
		if (ChildProcessEvent(event) == IEvent::DontContinue)
		{
			return IEvent::DontContinue;
		}

		if (event->Type == IEvent::Mouse)
		{
			MouseEvent *mouse = (MouseEvent*)event;

			if (Drawing::Rectangle(0, 0, bounds.GetWidth(), bounds.GetHeight()).Contains(mouse->Position))
			{
				if (mouse->State == MouseEvent::LeftDown || mouse->State == MouseEvent::RightDown)
				{
					MouseEventArgs args(mouse);
					mouseDownEvent.Invoke(this, args);

					parent->RequestFocus(this);

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::Move)
				{
					MouseEventArgs args(mouse);
					mouseMoveEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
				else if (mouse->State == MouseEvent::LeftUp || mouse->State == MouseEvent::RightUp)
				{
					MouseEventArgs args(mouse);
					mouseUpEvent.Invoke(this, args);

					return IEvent::DontContinue;
				}
			}

			mouse->Position = mousePositionBackup;
		}

		return IEvent::Continue;
	}
	//---------------------------------------------------------------------------
	void PictureBox::Render(Drawing::IRenderer *renderer)
	{
		if (!visible)
		{
			return;
		}
	
		if (backColor.A != 0)
		{
			renderer->SetRenderColor(backColor);
			renderer->Fill(bounds);
		}
		
		if (image != 0)
		{
			renderer->SetRenderColor(Drawing::Color::White());
			renderer->RenderTexture(image, bounds.GetPosition());
		}
	
		if (controls.size() > 0)
		{
			Drawing::Rectangle renderRect = renderer->GetRenderRectangle();
			renderer->SetRenderRectangle(clientArea + renderRect.GetPosition());
			
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i]->Render(renderer);
			}
			
			renderer->SetRenderRectangle(renderRect);
		}
	}
	//---------------------------------------------------------------------------
}