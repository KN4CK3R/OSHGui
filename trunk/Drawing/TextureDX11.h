#ifndef OSHGUI_DRAWING_TEXTUREDX11_H_
#define OSHGUI_DRAWING_TEXTUREDX11_H_

#include <d3d11.h>

#include "C:\Users\KN4CK3R\Desktop\gui\Drawing\ITexture.h"

namespace OSHGui
{
	namespace Drawing
	{
		class TextureDX11 : public ITexture
		{
		public:
			TextureDX11(ID3D11Device *device);
			virtual ~TextureDX11();
		
			ID3D11Texture2D* GetTexture();
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

			void Insert(const Drawing::Point &point, Drawing::TextureDX11 *texture);
			virtual void Insert(int x, int y, Drawing::ITexture *texture);
			
		private:
			ID3D11Device *device;
			
			D3D11_MAPPED_TEXTURE2D lock;
			
			ID3D11Texture2D *texture;
		};
	}
}

#endif