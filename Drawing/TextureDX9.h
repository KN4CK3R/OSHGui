#ifndef __OSHGUI_DRAWING_TEXTUREDX9_H__
#define __OSHGUI_DRAWING_TEXTUREDX9_H__

#include <d3d9.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\ITexture.h"

namespace OSHGui
{
	namespace Drawing
	{
		class TextureDX9 : public ITexture
		{
		public:
			TextureDX9(IDirect3DDevice9 *device);
			virtual ~TextureDX9();
		
			LPDIRECT3DTEXTURE9 GetTexture();
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

			void Insert(const Drawing::Point &point, Drawing::TextureDX9 *texture);
			virtual void Insert(int x, int y, Drawing::ITexture *texture);
			
		private:
			IDirect3DDevice9 *device;
			
			D3DLOCKED_RECT lock;
			
			LPDIRECT3DTEXTURE9 texture;
		};
	}
}

#endif