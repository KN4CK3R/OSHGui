#include "TextureAnimator.h"
#include "ITexture.h"
#include "..\Application.h"
#include "..\Exceptions.h"

namespace OSHGui
{
	namespace Drawing
	{
		std::list<TextureAnimator::TextureInfo> TextureAnimator::textureInfoList;
		bool TextureAnimator::anyFrameDirty = false;

		TextureAnimator::TextureInfo::TextureInfo(const std::shared_ptr<ITexture> &texture, ReplayMode replayMode)
		{
			this->replayMode = replayMode;
			bounceBackwards = false;
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
                    throw ArgumentOutOfRange(L"frame");
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
		int TextureAnimator::TextureInfo::GetNextFrame() const
		{
			switch (replayMode)
			{
				case Once:
					if (frame + 1 < GetFrameCount())
					{
						return frame + 1;
					}
					else
					{
						return -1;
					}
					break;
				case Loop:
					if (frame + 1 < GetFrameCount())
					{
						return frame + 1;
					}
					else
					{
						return 0;
					}
					break;
				case Bounce:
					if (bounceBackwards)
					{
						if (frame - 1 >= 0)
						{
							return frame - 1;
						}
						else
						{
							bounceBackwards = false;
							return frame + 1;
						}
					}
					else
					{
						if (frame + 1 < GetFrameCount())
						{
							return frame + 1;
						}
						else
						{
							bounceBackwards = true;
							return frame - 1;
						}
					}
			}

			return 0;
		}
		//---------------------------------------------------------------------------
		int TextureAnimator::TextureInfo::GetFrameCount() const
		{
			return frameCount;
		}
		//---------------------------------------------------------------------------
		TextureAnimator::ReplayMode TextureAnimator::TextureInfo::GetReplayMode() const
		{
			return replayMode;
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

				nextFrameChangeTime = Application::GetNow().Add(frameChangeInterval);

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

			std::list<TextureInfo>::iterator it = textureInfoList.begin();
			while (it != textureInfoList.end())
			{
				TextureInfo &textureInfo = *it;

				if (textureInfo.GetNextFrameChangeTime() < Application::GetNow())
				{
					int nextFrame = textureInfo.GetNextFrame();
					if (nextFrame != -1)
					{
						textureInfo.SetFrame(nextFrame);
					}
					else
					{
						it = textureInfoList.erase(it);
						continue;
					}

					if (textureInfo.IsFrameDirty())
					{ 
						anyFrameDirty = true;
					}
   				}

				++it;
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
		void TextureAnimator::Animate(const std::shared_ptr<ITexture> &texture, ReplayMode replayMode)
		{
			if (texture == 0)
			{
				throw ArgumentNullException(L"texture");
			}

			TextureInfo textureInfo(texture, replayMode);

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