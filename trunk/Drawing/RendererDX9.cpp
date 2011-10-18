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

			defaultFont = CreateNewFont(L"Arial", 14, false, false);
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
			device->GetFVF(&oldFVF);
		
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//device->SetRenderState(D3DRS_SCISSORTESTENABLE, true);

			device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
			device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

			device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			
			SetRenderRectangle(Drawing::Rectangle(GetRenderDimension()));
		}
		//---------------------------------------------------------------------------
		void RendererDX9::End()
		{
			Flush();
			
			device->SetFVF(oldFVF);
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
		const std::shared_ptr<ITexture> RendererDX9::CreateNewTexture(const Misc::UnicodeString &filename)
		{
			std::shared_ptr<TextureDX9> texture(new TextureDX9(device, filename));
			textureList.push_back(std::weak_ptr<TextureDX9>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<IFont> RendererDX9::CreateNewFont(const Misc::UnicodeString &fontName, int size, bool bold, bool italic)
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
		Size RendererDX9::MeasureText(const std::shared_ptr<IFont> &font, const Misc::UnicodeString &text)
		{
			if (font == 0)
			{
				return Size();
			}
			
			return font->MeasureText(text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, const Point &point, const Misc::UnicodeString &text)
		{
			RenderText(font, point.X, point.Y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, int x, int y, const Misc::UnicodeString &text)
		{
			RenderText(font, x, y, 1000, 100, text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, const Rectangle &rectangle, const Misc::UnicodeString &text)
		{
			RenderText(font, rectangle.GetLeft(), rectangle.GetTop(), rectangle.GetWidth(), rectangle.GetHeight(), text);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::UnicodeString &text)
		{
			if (font == 0)
			{
				return;
			}
			
			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			RECT clip = { x, y, x + w, y + h };
			std::static_pointer_cast<FontDX9>(font)->GetFont()->DrawTextW(0, text.c_str(), -1, &clip, DT_LEFT | DT_TOP, color.ARGB);
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
		void RendererDX9::Fill(const Rectangle &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight());
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
		void RendererDX9::FillGradient(const Rectangle &rect, Color &to)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), to);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::FillGradient(int x, int y, int w, int h, Color &to)
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