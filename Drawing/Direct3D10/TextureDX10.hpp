/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_TEXTUREDX10_HPP
#define OSHGUI_DRAWING_TEXTUREDX10_HPP

#include <d3d10.h>

#include "ITexture.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		class TextureDX10 : public ITexture
		{
		public:
			TextureDX10(ID3D10Device *device);
			virtual ~TextureDX10();
		
			ID3D10Texture2D* GetTexture();
			virtual bool IsLocked();

			virtual bool Create(const Drawing::Size &size);
		
			virtual void BeginUpdate();
			virtual void EndUpdate();
			
			void Clear();
			void Clear(const Drawing::Point &point);
			void Clear(int x, int y);
			void Clear(Drawing::Rectangle &rect);
			virtual void Clear(int x, int y, int w, int h);

			void Fill(D3DCOLOR color);
			void Fill(const Drawing::Point &point, D3DCOLOR color);
			void Fill(int x, int y, D3DCOLOR color);
			void Fill(Drawing::Rectangle &rect, D3DCOLOR color);
			virtual void Fill(int x, int y, int w, int h, D3DCOLOR color);

			void FillGradient(D3DCOLOR from, D3DCOLOR to, bool updown = true);
			void FillGradient(Drawing::Rectangle &rect, D3DCOLOR from, D3DCOLOR to, bool updown = true);
			virtual void FillGradient(int x, int y, int w, int h, D3DCOLOR from, D3DCOLOR to, bool updown = true);
			
			virtual void Rotate(int degrees);

			void Insert(const Drawing::Point &point, Drawing::TextureDX10 *texture);
			virtual void Insert(int x, int y, Drawing::ITexture *texture);
			
		private:
			ID3D10Device *device;
			
			D3D10_MAPPED_TEXTURE2D lock;
			
			ID3D10Texture2D *texture;
		};
	}
}

#endif