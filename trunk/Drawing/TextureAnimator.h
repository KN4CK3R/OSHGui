#ifndef __OSHGUI_DRAWING_IMAGEANIMATOR_H__
#define __OSHGUI_DRAWING_IMAGEANIMATOR_H__

#include <list>
#include <memory>
#include "..\Misc\DateTime.h"
#include "..\Misc\TimeSpan.h"

namespace OSHGui
{
	namespace Drawing
	{
		class ITexture;

		class TextureAnimator
		{
		public:
			static bool CanAnimate(const std::shared_ptr<ITexture> &texture);
			static void Animate(const std::shared_ptr<ITexture> &texture);
			static void StopAnimate(const std::shared_ptr<ITexture> &texture);
			static void UpdateFrames();

		private:
			static bool anyFrameDirty;

			class TextureInfo
			{
			public:
				TextureInfo(const std::shared_ptr<ITexture> &texture);

				const std::shared_ptr<ITexture>& GetTexture() const;
				bool IsAnimated() const;
				bool IsFrameDirty() const;
				void SetFrame(int frame);
				int GetFrame() const;
				int GetFrameCount() const;
				const Misc::TimeSpan& GetFrameChangeInterval() const;
				const Misc::DateTime& GetNextFrameChangeTime() const;

				void UpdateFrame();

			private:
				std::shared_ptr<ITexture> texture;
				bool animated;
				bool frameDirty;
				int frame;
				int frameCount;
				Misc::TimeSpan frameChangeInterval;
				Misc::DateTime nextFrameChangeTime;
			};
			static std::list<TextureInfo> textureInfoList;
		};
	}
}

#endif