/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2011 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RendererDX9.hpp"

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

			texture = 0;

			for (int i = 0; i < maxVertices; ++i)
			{
				vertices[i].z = 0.0f;
				vertices[i].rhw = 1.0f;
				vertices[i].u = 0.0f;
				vertices[i].v = 0.0f;
			}

			defaultFont = CreateNewFont("Arial", 14, false, false);

			device->CreateStateBlock(D3DSBT_ALL, &stateBlock);
		}
		//---------------------------------------------------------------------------
		RendererDX9::~RendererDX9()
		{

		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		void RendererDX9::SetRenderRectangle(const Rectangle &rect)
		{
			IRenderer::SetRenderRectangle(rect);
			//RECT scissor = { rect.GetLeft(), rect.GetTop(), rect.GetLeft() + rect.GetWidth(), rect.GetTop() + rect.GetHeight() };
			//device->SetScissorRect(&scissor);
		}
		//---------------------------------------------------------------------------
		const Size RendererDX9::GetRenderDimension() const
		{
			D3DVIEWPORT9 viewPort;
			device->GetViewport(&viewPort);
			return Size(viewPort.Width, viewPort.Height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void RendererDX9::Begin()
		{
			stateBlock->Capture();
		
			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			device->SetTexture(0, 0);
			device->SetVertexShader(0);
			device->SetPixelShader(0);

			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			device->SetRenderState(D3DRS_FOGENABLE, FALSE);
			device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			// setup texture addressing settings
			device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			// setup colour calculations
			device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

			// setup alpha calculations
			device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

			// setup filtering
			device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

			// disable texture stages we do not need.
			device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			// setup scene alpha blending
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
			device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);
			
			SetRenderRectangle(Drawing::Rectangle(GetRenderDimension()));
		}
		//---------------------------------------------------------------------------
		void RendererDX9::End()
		{
			Flush();
			
			stateBlock->Apply();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::PreReset()
		{
			{
				std::list<std::weak_ptr<TextureDX9> >::iterator it = textureList.begin();
				while (it != textureList.end())
				{
					std::weak_ptr<TextureDX9> &texture = *it;
					if (texture.expired())
					{
						it = textureList.erase(it);
					}
					else
					{
						//texture.lock()->GetTexture()->OnLostDevice();
						++it;
					}
				}
			}
			{
				std::list<std::weak_ptr<FontDX9> >::iterator it = fontList.begin();
				while (it != fontList.end())
				{
					std::weak_ptr<FontDX9> &font = *it;
					if (font.expired())
					{
						it = fontList.erase(it);
					}
					else
					{
						font.lock()->GetFont()->OnLostDevice();
						++it;
					}
				}
			}

			stateBlock->Release();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::PostReset()
		{
			{
				std::list<std::weak_ptr<TextureDX9> >::iterator it = textureList.begin();
				while (it != textureList.end())
				{
					std::weak_ptr<TextureDX9> &texture = *it;
					if (texture.expired())
					{
						it = textureList.erase(it);
					}
					else
					{
						//texture.lock()->GetTexture()->OnResetDevice();
						++it;
					}
				}
			}
			{
				std::list<std::weak_ptr<FontDX9> >::iterator it = fontList.begin();
				while (it != fontList.end())
				{
					std::weak_ptr<FontDX9> &font = *it;
					if (font.expired())
					{
						it = fontList.erase(it);
					}
					else
					{
						font.lock()->GetFont()->OnResetDevice();
						++it;
					}
				}
			}

			device->CreateStateBlock(D3DSBT_ALL, &stateBlock);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Flush()
		{
			if (verticesNum > 0)
			{
				device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, verticesNum / 3, &vertices[0], sizeof(Vertex2D));

				verticesNum = 0;
			}
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX9::CreateNewTexture(const Size &size, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			std::shared_ptr<TextureDX9> texture(new TextureDX9(device, size, frameCount, frameChangeInterval));
			textureList.push_back(std::weak_ptr<TextureDX9>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX9::CreateNewTexture(int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			std::shared_ptr<TextureDX9> texture(new TextureDX9(device, width, height, frameCount, frameChangeInterval));
			textureList.push_back(std::weak_ptr<TextureDX9>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX9::CreateNewTexture(const Misc::AnsiString &filename)
		{
			std::shared_ptr<TextureDX9> texture(new TextureDX9(device, filename));
			textureList.push_back(std::weak_ptr<TextureDX9>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<IFont> RendererDX9::CreateNewFont(const Misc::AnsiString &fontName, int size, bool bold, bool italic)
		{
			std::shared_ptr<FontDX9> font(new FontDX9(device, fontName, size, bold, italic));
			fontList.push_back(std::weak_ptr<FontDX9>(font));
			return font;
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point)
		{
			Drawing::Size size = texture->GetSize();
			RenderTexture(texture, point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y)
		{
			Drawing::Size size = texture->GetSize();
			RenderTexture(texture, x, y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, const Point &point, const Size &size)
		{
			RenderTexture(texture, point.Left, point.Top, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, const Rectangle &rect)
		{
			RenderTexture(texture, rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h)
		{
			if (texture == 0)
			{
				return;
			}
			
			std::shared_ptr<TextureDX9> temp = std::static_pointer_cast<TextureDX9>(texture);
			
			if (this->texture != temp)
			{
				Flush();
				device->SetTexture(0, temp->GetTexture());
				this->texture = temp;
			}

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			AddVertex(x, y, 0.0f, 0.0f);
			AddVertex(x + w, y, 1.0f, 0.0f);
			AddVertex(x, y + h, 0.0f, 1.0f);
			AddVertex(x + w, y, 1.0f, 0.0f);
			AddVertex(x + w, y + h, 1.0f, 1.0f);
			AddVertex(x, y + h, 0.0f, 1.0f);
		}
		//---------------------------------------------------------------------------
		Size RendererDX9::MeasureText(const std::shared_ptr<IFont> &font, const Misc::AnsiString &text)
		{
			if (font == 0)
			{
				return Size();
			}
			
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::AnsiString &text)
		{
			RenderText(font, point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::AnsiString &text)
		{
			RenderText(font, x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, const Rectangle &rectangle, const Misc::AnsiString &text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, const Point &location, const Size &size, const Misc::AnsiString &text)
		{
			RenderText(font, location.Left, location.Top, size.Width, size.Height, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text)
		{
			if (font == 0)
			{
				return;
			}
			
			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			RECT clip = { x, y, x + w, y + h };
			std::static_pointer_cast<FontDX9>(font)->GetFont()->DrawTextA(0, text.c_str(), -1, &clip, DT_LEFT | DT_TOP, color.ARGB);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(const Point &point)
		{
			Fill(point.X, point.Y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(const Point &point, const Size &size)
		{
			Fill(point.X, point.Y, size.Width, size.Height);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(int x, int y)
		{
			Fill(x, y, 1, 1);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(int x, int y, int w, int h)
		{
			if (texture != 0)
			{
				Flush();
				device->SetTexture(0, 0);
				texture = 0;
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
		void RendererDX9::Fill(const Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(const Point &point, const Size &size, const Color &to)
		{
			FillGradient(point.Left, point.Top, size.Width, size.Height, to);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(const Rectangle &rect, const Color &to)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), to);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(int x, int y, int w, int h, const Color &to)
		{
			if (texture != 0)
			{
				Flush();
				device->SetTexture(0, 0);
				texture = 0;
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

			++verticesNum;
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

			++verticesNum;
		}
		//---------------------------------------------------------------------------
	}
}