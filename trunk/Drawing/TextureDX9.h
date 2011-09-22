#ifndef __OSHGUI_DRAWING_TEXTUREDX9_H__
#define __OSHGUI_DRAWING_TEXTUREDX9_H__

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include "ITexture.h"

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

			virtual bool Create(const Size &size);
			virtual bool Create(int width, int height);
			virtual bool LoadFromFile(const Misc::UnicodeString &filename);
		
			virtual void BeginUpdate();
			virtual void EndUpdate();
			
			virtual void Clear();
			virtual void Clear(const Point &point);
			virtual void Clear(int x, int y);
			virtual void Clear(const Rectangle &rect);
			virtual void Clear(int x, int y, int w, int h);

			virtual void Fill(Color color);
			virtual void Fill(const Point &point, Color color);
			virtual void Fill(int x, int y, Color color);
			virtual void Fill(const Rectangle &rect, Color color);
			virtual void Fill(int x, int y, int w, int h, Color color);

			virtual void FillGradient(Color from, Color to, bool updown = true);
			virtual void FillGradient(const Rectangle &rect, Color from, Color to, bool updown = true);
			virtual void FillGradient(int x, int y, int w, int h, Color from, Color to, bool updown = true);
			
			virtual void Rotate(int degrees);

			virtual void Insert(const Point &point, ITexture *texture);
			virtual void Insert(int x, int y, ITexture *texture);
			
		private:
			IDirect3DDevice9 *device;
			
			D3DLOCKED_RECT lock;
			
			LPDIRECT3DTEXTURE9 texture;
		};
	}
}

#endif