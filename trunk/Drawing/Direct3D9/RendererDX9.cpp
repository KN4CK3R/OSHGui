/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RendererDX9.hpp"
#define NOMINMAX
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

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

			texture = nullptr;
			sprite = nullptr;

			for (int i = 0; i < maxVertices; ++i)
			{
				vertices[i].z = 0.0f;
				vertices[i].u = 0.0f;
				vertices[i].v = 0.0f;
			}

			InitializeDevice();

			defaultFont = CreateNewFont("Arial", 14, false, false);
		}
		//---------------------------------------------------------------------------
		RendererDX9::~RendererDX9()
		{
			sprite->Release();
			stateBlockBackup->Release();
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
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
		void RendererDX9::InitializeDevice()
		{
			flushSprite = false;

			if (sprite == nullptr)
			{
				D3DXCreateSprite(device, &sprite);
			}
			else
			{
				sprite->OnResetDevice();
			}

			device->CreateStateBlock(D3DSBT_ALL, &stateBlockBackup);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Begin()
		{
			stateBlockBackup->Capture();

			sprite->Begin(D3DXSPRITE_ALPHABLEND);

			device->SetFVF(D3DFVF_XYZ | D3DFVF_SPECULAR | D3DFVF_TEX1);
			device->SetTexture(0, nullptr);
			device->SetVertexShader(nullptr);
			device->SetPixelShader(nullptr);
			
			SetRenderRectangle(Drawing::Rectangle(GetRenderDimension()));
		}
		//---------------------------------------------------------------------------
		void RendererDX9::End()
		{
			Flush();

			sprite->End();

			stateBlockBackup->Apply();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::PreReset()
		{
			for (auto it = textureList.begin(); it != textureList.end();)
			{
				std::weak_ptr<TextureDX9> &texture = *it;
				if (texture.expired())
				{
					it = textureList.erase(it);
				}
				else
				{
					texture.lock()->PreReset();
					++it;
				}
			}
			for (auto it = fontList.begin(); it != fontList.end();)
			{
				std::weak_ptr<FontDX9> &font = *it;
				if (font.expired())
				{
					it = fontList.erase(it);
				}
				else
				{
					font.lock()->PreReset();
					++it;
				}
			}

			sprite->OnLostDevice();

			stateBlockBackup->Release();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::PostReset()
		{
			for (auto it = textureList.begin(); it != textureList.end();)
			{
				std::weak_ptr<TextureDX9> &texture = *it;
				if (texture.expired())
				{
					it = textureList.erase(it);
				}
				else
				{
					texture.lock()->PostReset();
					++it;
				}
			}
			for (auto it = fontList.begin(); it != fontList.end();)
			{
				std::weak_ptr<FontDX9> &font = *it;
				if (font.expired())
				{
					it = fontList.erase(it);
				}
				else
				{
					font.lock()->PostReset();
					++it;
				}
			}

			InitializeDevice();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Flush()
		{
			if (verticesNum > 0)
			{
				device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, verticesNum / 3, vertices, sizeof(Vertex2D));

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
		void RendererDX9::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h)
		{
			if (texture == nullptr)
			{
				return;
			}
			
			std::shared_ptr<TextureDX9> temp = std::static_pointer_cast<TextureDX9>(texture);
			
			/*if (this->texture != temp)
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
			AddVertex(x, y + h, 0.0f, 1.0f);*/
			
			D3DXVECTOR3 v = D3DXVECTOR3(x, y, 0.0f);
			sprite->Draw(temp->GetTexture(), nullptr, nullptr, &v, 0xFFFFFFFF);
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text)
		{
			if (font == 0 || text.empty())
			{
				return;
			}
			
			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			RECT clip = { x, y, x + w, y + h };
			std::static_pointer_cast<FontDX9>(font)->GetFont()->DrawTextA(sprite, text.c_str(), text.length(), &clip, DT_LEFT | DT_TOP, color.ARGB);

			flushSprite = true;
		}
		//---------------------------------------------------------------------------
		void RendererDX9::Fill(int x, int y, int w, int h)
		{
			if (texture != 0)
			{
				Flush();
				device->SetTexture(0, nullptr);
				texture = nullptr;
			}
			if (flushSprite)
			{
				sprite->Flush();
				flushSprite = false;
				device->SetTexture(0, nullptr);
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
		void RendererDX9::BeginLines()
		{
			Flush();
		}
		//---------------------------------------------------------------------------
		void RendererDX9::RenderLine(int x1, int y1, int x2, int y2)
		{
			if (verticesNum >= maxVertices - 2)
			{
				EndLines();
			}

			vertices[verticesNum].x = (float)x1;
			vertices[verticesNum].y = (float)y1;
			vertices[verticesNum].color = color.ARGB;

			++verticesNum;

			vertices[verticesNum].x = (float)x2;
			vertices[verticesNum].y = (float)y2;
			vertices[verticesNum].color = color.ARGB;

			++verticesNum;
		}
		//---------------------------------------------------------------------------
		void RendererDX9::EndLines()
		{
			if (verticesNum > 0)
			{
				device->DrawPrimitiveUP(D3DPT_LINELIST, verticesNum / 2, vertices, sizeof(Vertex2D));

				verticesNum = 0;
			}
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