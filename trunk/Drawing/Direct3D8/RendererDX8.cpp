/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RendererDX8.hpp"
#define NOMINMAX
#include <d3dx8.h>
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		RendererDX8::RendererDX8(IDirect3DDevice8 *device)
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

			D3DCAPS8 caps;
			device->GetDeviceCaps(&caps);

			supportNonSquareTex = !(caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY);

			supportNPOTTex = !(caps.TextureCaps & D3DPTEXTURECAPS_POW2) || (caps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL);

			InitializeDevice();

			defaultFont = CreateNewFont("Arial", 14, false, false);
		}
		//---------------------------------------------------------------------------
		RendererDX8::~RendererDX8()
		{
			sprite->Release();
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const Size RendererDX8::GetRenderDimension() const
		{
			D3DVIEWPORT8 viewPort;
			device->GetViewport(&viewPort);
			return Size(viewPort.Width, viewPort.Height);
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		Size RendererDX8::AdjustSize(Size size) const
		{
			if (!supportNPOTTex)
			{
				auto PowerSize = [](int size) -> int
				{
					if ((size & (size - 1)) || !size)
					{
						int log = 0;

						while (size >>= 1)
						{
							++log;
						}

						size = (2 << log);
					}
					return size;
				};

				size.Width  = PowerSize(size.Width);
				size.Height = PowerSize(size.Height);
			}
			if (!supportNonSquareTex)
			{
				size.Width = size.Height = std::max(size.Width, size.Height);
			}

			return size;
		}
		//---------------------------------------------------------------------------
		void RendererDX8::InitializeDevice()
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

			device->CreateStateBlock(D3DSBT_ALL, &stateBlockHandle);
		}
		//---------------------------------------------------------------------------
		void RendererDX8::Begin()
		{
			device->CaptureStateBlock(stateBlockHandle);

			device->SetTexture(0, nullptr);
			device->SetPixelShader(0);
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			sprite->Begin();
			
			SetRenderRectangle(Drawing::Rectangle(GetRenderDimension()));
		}
		//---------------------------------------------------------------------------
		void RendererDX8::End()
		{
			Flush();

			sprite->End();

			device->ApplyStateBlock(stateBlockHandle);
		}
		//---------------------------------------------------------------------------
		void RendererDX8::PreReset()
		{
			for (auto it = textureList.begin(); it != textureList.end();)
			{
				auto texture = *it;
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
				auto font = *it;
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

			sprite->OnResetDevice();

			device->DeleteStateBlock(stateBlockHandle);
		}
		//---------------------------------------------------------------------------
		void RendererDX8::PostReset()
		{
			for (auto it = textureList.begin(); it != textureList.end();)
			{
				auto texture = *it;
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
				auto font = *it;
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
		void RendererDX8::Flush()
		{
			if (verticesNum > 0)
			{
				DWORD fvf;
				device->GetVertexShader(&fvf);
				device->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
				device->SetTexture(0, nullptr);
				device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, verticesNum / 3, vertices, sizeof(Vertex2D));
				device->SetVertexShader(fvf);
				
				verticesNum = 0;
			}
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX8::CreateNewTexture(const Size &size, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			std::shared_ptr<TextureDX8> texture(new TextureDX8(this, device, size, frameCount, frameChangeInterval));
			textureList.push_back(std::weak_ptr<TextureDX8>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX8::CreateNewTexture(int width, int height, int frameCount, Misc::TimeSpan frameChangeInterval)
		{
			std::shared_ptr<TextureDX8> texture(new TextureDX8(this, device, Size(width, height), frameCount, frameChangeInterval));
			textureList.push_back(std::weak_ptr<TextureDX8>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<ITexture> RendererDX8::CreateNewTexture(const Misc::AnsiString &filename)
		{
			std::shared_ptr<TextureDX8> texture(new TextureDX8(this, device, filename));
			textureList.push_back(std::weak_ptr<TextureDX8>(texture));
			return texture;
		}
		//---------------------------------------------------------------------------
		const std::shared_ptr<IFont> RendererDX8::CreateNewFont(const Misc::AnsiString &fontName, int size, bool bold, bool italic)
		{
			std::shared_ptr<FontDX8> font(new FontDX8(device, fontName, size, bold, italic));
			fontList.push_back(std::weak_ptr<FontDX8>(font));
			return font;
		}
		//---------------------------------------------------------------------------
		void RendererDX8::RenderTexture(const std::shared_ptr<ITexture> &texture, int x, int y, int w, int h)
		{
			if (texture == nullptr)
			{
				return;
			}
			
			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			D3DXVECTOR2 screenPosition(x, y);
			float scaleWidth = static_cast<float>(w) / texture->GetSize().Width;
			float scaleHeight = static_cast<float>(h) / texture->GetSize().Height;
			D3DXVECTOR2 scaling = scaleWidth < scaleHeight ? D3DXVECTOR2(scaleWidth, scaleWidth) : D3DXVECTOR2(scaleHeight, scaleHeight);

			sprite->Draw(std::static_pointer_cast<TextureDX8>(texture)->GetTexture(), nullptr, &scaling, nullptr, 0.0f, &screenPosition, 0xFFFFFFFF);
		}
		//---------------------------------------------------------------------------
		void RendererDX8::RenderText(const std::shared_ptr<IFont> &font, int x, int y, int w, int h, const Misc::AnsiString &text)
		{
			if (font == nullptr || text.empty())
			{
				return;
			}
			
			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();
			
			RECT clip = { x, y, x + w, y + h };
			std::static_pointer_cast<FontDX8>(font)->GetFont()->DrawTextA(text.c_str(), text.length(), &clip, DT_LEFT | DT_TOP, color.ARGB);

			flushSprite = true;
		}
		//---------------------------------------------------------------------------
		void RendererDX8::Fill(int x, int y, int w, int h)
		{
			if (texture)
			{
				Flush();
				device->SetTexture(0, nullptr);
				texture = nullptr;
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
		void RendererDX8::FillGradient(int x, int y, int w, int h, Color to)
		{
			if (texture)
			{
				Flush();
				device->SetTexture(0, nullptr);
				texture = nullptr;
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
		void RendererDX8::BeginLines()
		{
			Flush();
		}
		//---------------------------------------------------------------------------
		void RendererDX8::RenderLine(int x1, int y1, int x2, int y2)
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
		void RendererDX8::EndLines()
		{
			if (verticesNum > 0)
			{
				DWORD fvf;
				device->GetVertexShader(&fvf);
				device->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);
				device->DrawPrimitiveUP(D3DPT_LINELIST, verticesNum / 2, vertices, sizeof(Vertex2D));
				device->SetVertexShader(fvf);

				verticesNum = 0;
			}
		}
		//---------------------------------------------------------------------------
		void RendererDX8::AddVertex(int x, int y)
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
		void RendererDX8::AddVertex(int x, int y, float u, float v)
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