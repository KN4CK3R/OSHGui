#include "TextureAnimator.h"
#include "ITexture.h"
#include "..\Application.h"

namespace OSHGui
{
	namespace Drawing
	{
		std::list<TextureAnimator::TextureInfo> TextureAnimator::textureInfoList;
		bool TextureAnimator::anyFrameDirty = false;

		TextureAnimator::TextureInfo::TextureInfo(const std::shared_ptr<ITexture> &texture)
		{
			frameDirty = false;
			this->texture = texture;
			frame = 0;
            animated = TextureAnimator::CanAnimate(texture);

            if (animated)
			{
                frameCount = texture->GetFrameCount(); 
                frameChangeInterval = texture->GetFrameChangeInterval();
            }
            else
			{
                frameCount = 1;
            }
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture>& TextureAnimator::TextureInfo::GetTexture() const
		{
			return texture;
		}
		//---------------------------------------------------------------------------
		bool TextureAnimator::TextureInfo::IsAnimated() const
		{
			return animated;
		}
		//---------------------------------------------------------------------------
		bool TextureAnimator::TextureInfo::IsFrameDirty() const
		{
			return frameDirty;
		}
		//---------------------------------------------------------------------------
		void TextureAnimator::TextureInfo::SetFrame(int frame)
		{
			if (this->frame != frame)
			{
				if (frame < 0 || frame >= GetFrameCount())
				{
                    frame = 0;
                }
 
                if (IsAnimated())
				{
                    this->frame = frame;
					frameDirty = true;
                }
			}
		}
		//---------------------------------------------------------------------------
		int TextureAnimator::TextureInfo::GetFrame() const
		{
			return frame;
		}
		//---------------------------------------------------------------------------
		int TextureAnimator::TextureInfo::GetFrameCount() const
		{
			return frameCount;
		}
		//---------------------------------------------------------------------------
		const Misc::TimeSpan& TextureAnimator::TextureInfo::GetFrameChangeInterval() const
		{
			return frameChangeInterval;
		}
		//---------------------------------------------------------------------------
		const Misc::DateTime& TextureAnimator::TextureInfo::GetNextFrameChangeTime() const
		{
			return nextFrameChangeTime;
		}
		//---------------------------------------------------------------------------
		void TextureAnimator::TextureInfo::UpdateFrame()
		{
			if (frameDirty)
			{
				texture->SelectActiveFrame(frame);

				nextFrameChangeTime = Application::Now.Add(frameChangeInterval);

				frameDirty = false;
			}
		}
		//---------------------------------------------------------------------------
		bool TextureAnimator::CanAnimate(const std::shared_ptr<ITexture> &texture)
		{
			if (texture == 0)
			{
				return false;
			}

			return texture->GetFrameCount() > 1;
		}
		//---------------------------------------------------------------------------
		void TextureAnimator::UpdateFrames()
		{
			if (textureInfoList.empty())
			{
				return;
			}

			for (std::list<TextureInfo>::iterator it = textureInfoList.begin(); it != textureInfoList.end(); ++it)
			{
				TextureInfo &textureInfo = *it;

				if (textureInfo.GetNextFrameChangeTime() < Application::Now)
				{
					int nextFrame = textureInfo.GetFrame() + 1;
					if (nextFrame < textureInfo.GetFrameCount())
					{
						textureInfo.SetFrame(nextFrame);
					}
					else
					{
						textureInfo.SetFrame(0);
					}

					if (textureInfo.IsFrameDirty())
					{ 
						anyFrameDirty = true;
					}
   				}
			}

			if (!anyFrameDirty)
			{
				return;
			}

			for (std::list<TextureInfo>::iterator it = textureInfoList.begin(); it != textureInfoList.end(); ++it)
			{
				it->UpdateFrame();
			}
        }
		//---------------------------------------------------------------------------
		void TextureAnimator::Animate(const std::shared_ptr<ITexture> &texture)
		{
			if (texture == 0)
			{
				return;
			}

			TextureInfo textureInfo(texture);

			StopAnimate(texture);

			if (textureInfo.IsAnimated())
			{
				textureInfoList.push_back(textureInfo);
			}
		}
		//---------------------------------------------------------------------------
		void TextureAnimator::StopAnimate(const std::shared_ptr<ITexture> &texture)
		{
			if (texture == 0 || textureInfoList.empty())
			{
                return;
            }

			for (std::list<TextureInfo>::iterator it = textureInfoList.begin(); it != textureInfoList.end(); ++it)
			{
				if (it->GetTexture() == texture)
				{
					textureInfoList.erase(it);
					break;
				}
			}
		}
		//---------------------------------------------------------------------------
	}
}