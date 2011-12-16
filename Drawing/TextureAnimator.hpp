/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_IMAGEANIMATOR_HPP
#define OSHGUI_DRAWING_IMAGEANIMATOR_HPP

#include <list>
#include <functional>
#include <memory>
#include "../Exports.hpp"
#include "../Misc/DateTime.hpp"
#include "../Misc/TimeSpan.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class ITexture;

		class OSHGUI_EXPORT TextureAnimator
		{
		public:
			enum ReplayMode
			{
				Loop,
				Bounce,
				Once
			};

			static bool CanAnimate(const std::shared_ptr<ITexture> &texture);
			static void Animate(const std::shared_ptr<ITexture> &texture, ReplayMode replayMode);
			static void Animate(const std::shared_ptr<ITexture> &texture, ReplayMode replayMode, const std::function<void(const std::shared_ptr<ITexture> &texture)> &frameChangeFunction);
			static void StopAnimate(const std::shared_ptr<ITexture> &texture);
			static void UpdateFrames();

		private:		
			static bool anyFrameDirty;

			class TextureInfo
			{
			public:
				TextureInfo(const std::shared_ptr<ITexture> &texture, ReplayMode replayMode, const std::function<void(const std::shared_ptr<ITexture> &texture)> &frameChangeFunction);

				const std::shared_ptr<ITexture>& GetTexture() const;
				bool IsFrameDirty() const;
				void SetFrame(int frame);
				int GetFrame() const;
				int GetNextFrame() const;
				int GetFrameCount() const;
				ReplayMode GetReplayMode() const;
				const Misc::TimeSpan& GetFrameChangeInterval() const;
				const Misc::DateTime& GetNextFrameChangeTime() const;

				void UpdateFrame();

			private:
				std::shared_ptr<ITexture> texture;
				bool frameDirty;
				mutable bool bounceBackwards;
				int frame;
				int frameCount;
				Misc::TimeSpan frameChangeInterval;
				Misc::DateTime nextFrameChangeTime;
				ReplayMode replayMode;
				std::function<void(const std::shared_ptr<ITexture> &texture)> frameChangeFunction;
				
			};
			static std::list<TextureInfo> textureInfoList;
		};
	}
}

#endif