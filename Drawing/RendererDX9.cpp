#include "RendererDX9.h"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		RendererDX9::RendererDX9(IDirect3DDevice9 *device)
		{
			this->device = device;
			
			verticesNum = 0;

			texture = NULL;
			D3DXCreateSprite(device, &sprite);

			for (int i = 0; i < maxVertices; i++)
			{
				vertices[i].z = 0.0f;
				vertices[i].rhw = 1.0f;
				vertices[i].u = 0.0f;
				vertices[i].v = 0.0f;
			}
		}
		//---------------------------------------------------------------------------
		RendererDX9::~RendererDX9()
		{
			sprite->Release();
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void RendererDX9::Begin()
		{
			device->GetFVF(&oldFVF);
		
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

			//sprite->Begin(D3DXSPRITE_ALPHABLEND);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::End()
		{
			Flush();
			//sprite->End();
			
			device->SetFVF(oldFVF);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Flush()
		{
			//sprite->Flush();

			if (verticesNum > 0)
			{
				device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, verticesNum / 3, &vertices[0], sizeof(Vertex2D));

				verticesNum = 0;
			}
		}
		//---------------------------------------------------------------------------
		ITexture* RendererDX9::CreateNewTexture()
		{
			TextureDX9 *newTexture = new TextureDX9(device);
			return newTexture;
		}
		//---------------------------------------------------------------------------
		IFont* RendererDX9::CreateNewFont()
		{
			FontDX9 *newFont = new FontDX9(device);
			return newFont;
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(ITexture *texture, const Point &point)
		{
			RenderTexture(texture, point.X, point.Y);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(ITexture *texture, int x, int y)
		{
			if (texture == NULL)
			{
				return;
			}
			
			TextureDX9 *temp = (TextureDX9*)texture;
			
			if (this->texture != temp)
			{
				Flush();
				device->SetTexture(0, temp->GetTexture());
				this->texture = temp;
			}

			Size size = temp->GetSize();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			AddVertex(x, y, 0.0f, 0.0f);
			AddVertex(x + size.Width, y, 1.0f, 0.0f);
			AddVertex(x, y + size.Height, 0.0f, 1.0f);
			AddVertex(x + size.Width, y, 1.0f, 0.0f);
			AddVertex(x + size.Width, y + size.Height, 1.0f, 1.0f);
			AddVertex(x, y + size.Height, 0.0f, 1.0f);
		}
		//---------------------------------------------------------------------------
		Size RendererDX9::MeasureText(IFont *font, const char *text)
		{
			if (font == NULL || text == NULL || strlen(text) == 0)
			{
				return Size();
			}
			
			RECT rect = { 0, 0, 0, 0 };
			((FontDX9*)font)->GetFont()->DrawText(NULL, text, -1, &rect, DT_CALCRECT, 0);
			
			for (int i = strlen(text) - 1; i > 0; i--)
			{
				rect.right += font->MeasureCharacter(' ');
			}

			return Size(rect.right, rect.bottom);
		}
		//---------------------------------------------------------------------------
		Size RendererDX9::MeasureTextEx(IFont *font, const char *text, ...)
		{
			return MeasureText(font, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(IFont *font, const Point &point, const char *text)
		{
			RenderText(font, point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(IFont *font, int x, int y, const char *text)
		{
			RenderText(font, x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(IFont *font, Rectangle &rectangle, const char *text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(IFont *font, int x, int y, int w, int h, const char *text)
		{
			if (font == NULL || text == NULL)
			{
				return;
			}
			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			RECT clip = { x, y, x + w, y + h };
			((FontDX9*)font)->GetFont()->DrawText(NULL, text, -1, &clip, DT_LEFT | DT_TOP | DT_SINGLELINE, color.ARGB); ///DT_NOCLIP
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTextEx(IFont *font, const Point &point, const char *text, ...)
		{
			va_list arguments;
			char buffer[1024];

			va_start(arguments, text);
				_vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), text, arguments);
			va_end(arguments);
			
			RenderText(font, point.X, point.Y, 1000, 100, buffer);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTextEx(IFont *font, int x, int y, const char *text, ...)
		{
			va_list arguments;
			char buffer[1024];

			va_start(arguments, text);
				_vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), text, arguments);
			va_end(arguments);
			
			RenderText(font, x, y, 1000, 100, buffer);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTextEx(IFont *font, Rectangle &rect, const char *text, ...)
		{
			va_list arguments;
			char buffer[1024];

			va_start(arguments, text);
				_vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), text, arguments);
			va_end(arguments);
			
			RenderText(font, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), buffer);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTextEx(IFont *font, int x, int y, int w, int h, const char *text, ...)
		{
			va_list arguments;
			char buffer[1024];

			va_start(arguments, text);
				_vsnprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), text, arguments);
			va_end(arguments);
			
			RenderText(font, x, y, w, h, buffer);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(const Point &point)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(int x, int y)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(int x, int y, int w, int h)
		{
			if (texture != NULL)
			{
				Flush();
				device->SetTexture(0, NULL);
				texture = NULL;
			}

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			AddVertex(x, y);
			AddVertex(x + w, y);
			AddVertex(x, y + h);
			AddVertex(x + w, y);
			AddVertex(x + w, y + h);
			AddVertex(x, y + h);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(const Point &point, Color &to)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(int x, int y, Color &to)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(Rectangle &rect, Color &to)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(int x, int y, int w, int h, Color &to)
		{
			if (texture != NULL)
			{
				Flush();
				device->SetTexture(0, NULL);
				texture = NULL;
			}

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			Color bckColor = color;
			
			AddVertex(x, y);
			AddVertex(x + w, y);

			SetRenderColor(to);

			AddVertex(x, y + h);

			AddVertex(x + w, y + h);
			AddVertex(x, y + h);

			SetRenderColor(bckColor);

			AddVertex(x + w, y);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::AddVertex(int x, int y)
		{
			if (verticesNum >= maxVertices - 1)
			{
				Flush();
			}

			vertices[verticesNum].x = (float)x;
			vertices[verticesNum].y = (float)y;
			vertices[verticesNum].color = color.ARGB;

			verticesNum++;
		}
		//---------------------------------------------------------------------------
		void RendererDX9::AddVertex(int x, int y, float u, float v)
		{
			if (verticesNum >= maxVertices - 1)
			{
				Flush();
			}

			vertices[verticesNum].x = (float)x - 0.5f;
			vertices[verticesNum].y = (float)y - 0.5f;
			vertices[verticesNum].u = u;
			vertices[verticesNum].v = v;
			vertices[verticesNum].color = color.ARGB;

			verticesNum++;
		}
		//---------------------------------------------------------------------------
	}
}