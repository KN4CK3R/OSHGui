/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <d3d8.h>
#include "FontDX8.hpp"
#include "../Color.hpp"
#include "../../Misc/Exceptions.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		FontDX8::FontDX8(IDirect3DDevice8 *device, const Misc::AnsiString &name, int size, bool bold, bool italic)
			: IFont(name, size, bold, italic),
			  device(device),
			  vertexBuffer(nullptr)
		{
			Texture.Data = nullptr;

			Initialize();
		}
		//---------------------------------------------------------------------------
		FontDX8::~FontDX8()
		{
			Destroy();
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		void FontDX8::Initialize()
		{
			device->CreateStateBlock(D3DSBT_ALL, &backupStateBlock);

			if (FAILED(device->CreateVertexBuffer(maxVertices * sizeof(Vertex2D), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vertexBuffer)))
			{
				return;
			}

			device->BeginStateBlock();
			device->SetRenderState(D3DRS_ZENABLE, FALSE);
			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA);
			device->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
			device->SetRenderState(D3DRS_ALPHAREF,         0x08);
			device->SetRenderState(D3DRS_ALPHAFUNC,  D3DCMP_GREATEREQUAL);
			device->SetRenderState(D3DRS_FILLMODE,   D3DFILL_SOLID);
			device->SetRenderState(D3DRS_CULLMODE,   D3DCULL_CCW);
			device->SetRenderState(D3DRS_STENCILENABLE,    FALSE);
			device->SetRenderState(D3DRS_CLIPPING,         TRUE);
			device->SetRenderState(D3DRS_EDGEANTIALIAS,    FALSE);
			device->SetRenderState(D3DRS_CLIPPLANEENABLE,  FALSE);
			device->SetRenderState(D3DRS_VERTEXBLEND,      FALSE);
			device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			device->SetRenderState(D3DRS_FOGENABLE,        FALSE);
			device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
			device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTEXF_POINT);
			device->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTEXF_POINT);
			device->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTEXF_NONE);
			device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			device->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
			device->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
			device->EndStateBlock(&drawStateBlock);

			Texture.Scale = 1.0f;

			int textureSize = GetSize() > 40 ? 1024 : GetSize() > 20 ? 512 : 256;
			Texture.SizeF = SizeF(textureSize, textureSize);

			D3DCAPS8 d3dCaps;
			device->GetDeviceCaps(&d3dCaps);
			if (textureSize > d3dCaps.MaxTextureWidth)
			{
				Texture.Scale = d3dCaps.MaxTextureWidth / (float)textureSize;
				Texture.SizeF = SizeF(d3dCaps.MaxTextureWidth, d3dCaps.MaxTextureWidth);
			}

			if (FAILED(device->CreateTexture(Texture.SizeF.Width, Texture.SizeF.Height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A4R4G4B4, D3DPOOL_DEFAULT, &Texture.Data)))
			{
				return;
			}

			BITMAPINFO bmi;
			ZeroMemory(&bmi.bmiHeader, sizeof(BITMAPINFOHEADER));
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = Texture.SizeF.Width;
			bmi.bmiHeader.biHeight = -Texture.SizeF.Height;
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biCompression = BI_RGB;
			bmi.bmiHeader.biBitCount = 32;

			auto hdc = CreateCompatibleDC(NULL);
			int *bitmapData;
			auto bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&bitmapData, NULL, 0);
			SetMapMode(hdc, MM_TEXT);

			auto font = CreateFontA(GetSize(), 0, 0, 0, IsBold() ? FW_BOLD : FW_NORMAL, IsItalic(), 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, NONANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, GetName().c_str());
			if (!font)
			{
				return;
			}

			SelectObject(hdc, bitmap);
			SelectObject(hdc, font);

			SetTextColor(hdc, RGB(255,255,255));
			SetBkColor(hdc, 0x00000000);
			SetTextAlign(hdc, TA_TOP);

			auto x = 0;
			auto y = 0;

			SIZE extend;
			GetTextExtentPoint32A(hdc, " ", 1, &extend);
			x = Texture.Space = ceil(extend.cy * 0.1f);

			for (char c = 32; c < 127; ++c)
			{
				char str[2] = { c, 0 };
				GetTextExtentPoint32A(hdc, str, 1, &extend);

				if (x + extend.cx + 1 > Texture.SizeF.Width)
				{
					x = Texture.Space;
					y += extend.cy + 1;
				}

				ExtTextOutA(hdc, x, y, ETO_OPAQUE, nullptr, str, 1, nullptr);

				Texture.Coordinates[c-32][0] = (float)(x + 0         - Texture.Space) / Texture.SizeF.Width;
				Texture.Coordinates[c-32][1] = (float)(y + 0         + 0            ) / Texture.SizeF.Height;
				Texture.Coordinates[c-32][2] = (float)(x + extend.cx + Texture.Space) / Texture.SizeF.Width;
				Texture.Coordinates[c-32][3] = (float)(y + extend.cy + 0            ) / Texture.SizeF.Height;

				x += extend.cx + (2 * Texture.Space);
			}

			D3DLOCKED_RECT d3dlr;
			Texture.Data->LockRect(0, &d3dlr, 0, 0);
			auto raw = (BYTE*)d3dlr.pBits;

			for (y = 0; y < Texture.SizeF.Height; ++y)
			{
				auto raw16 = reinterpret_cast<WORD*>(raw);
				for (x = 0; x < Texture.SizeF.Width; ++x)
				{
					auto alpha = (BYTE)((bitmapData[Texture.SizeF.Width*y + x] & 0xff) >> 4);
					if (alpha > 0)
					{
						*raw16++ = (alpha << 12) | 0x0fff;
					}
					else
					{
						*raw16++ = 0x0000;
					}
				}
				raw += d3dlr.Pitch;
			}

			Texture.Data->UnlockRect(0);

			DeleteObject(bitmap);
			DeleteDC(hdc);
			DeleteObject(font);
		}
		//---------------------------------------------------------------------------
		void FontDX8::Destroy()
		{
			if (vertexBuffer)
			{
				vertexBuffer->Release();
				vertexBuffer = nullptr;
			}
			if (Texture.Data)
			{
				Texture.Data->Release();
				Texture.Data = nullptr;
			}
		}
		//---------------------------------------------------------------------------
		const SizeF FontDX8::MeasureText(const Misc::AnsiString &str)
		{
			if (str.length() == 0)
			{
				return SizeF(0, GetSize());
			}

			auto lineWidth  = 0.0f;
			auto lineHeight = (Texture.Coordinates[0][3] - Texture.Coordinates[0][1]) * Texture.SizeF.Height;

			SizeF size(0, lineHeight);

			for (auto it = std::begin(str); it != std::end(str); ++it)
			{
				auto c = *it;
				if (c == '\n')
				{
					lineWidth = 0.0f;
					size.Height  += lineHeight;
				}

				if (c - 32 < 0 || c - 32 >= 128 - 32)
				{
					continue;
				}

				auto tx1 = Texture.Coordinates[c - 32][0];
				auto tx2 = Texture.Coordinates[c - 32][2];

				lineWidth += (tx2 - tx1) * Texture.SizeF.Width - 2 * Texture.Space;

				if (lineWidth > size.Width)
				{
					size.Width = lineWidth;
				}
			}

			return size;
		}
		//---------------------------------------------------------------------------
		void FontDX8::RenderText(int x, int y, Color color, const Misc::AnsiString &text)
		{
			device->CaptureStateBlock(backupStateBlock);

			device->ApplyStateBlock(drawStateBlock);
			device->SetVertexShader(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1);
			device->SetPixelShader(NULL);
			device->SetTexture(0, Texture.Data);
			device->SetStreamSource(0, vertexBuffer, sizeof(Vertex2D));

			x -= Texture.Space;
			auto startX = x;

			Vertex2D *vertices = nullptr;
			auto triangles = 0;
			vertexBuffer->Lock(0, 0, (BYTE**)&vertices, 0);

			for (auto it = std::begin(text); it != std::end(text); ++it)
			{
				auto c = *it;

				if (c == '\n')
				{
					x = startX;
					y += (Texture.Coordinates[0][3] - Texture.Coordinates[0][1]) * Texture.SizeF.Height;
				}

				if (c - 32 < 0 || c - 32 >= 128 - 32)
				{
					continue;
				}

				auto tx1 = Texture.Coordinates[c - 32][0];
				auto ty1 = Texture.Coordinates[c - 32][1];
				auto tx2 = Texture.Coordinates[c - 32][2];
				auto ty2 = Texture.Coordinates[c - 32][3];

				auto w = (tx2 - tx1) * Texture.SizeF.Width / Texture.Scale;
				auto h = (ty2 - ty1) * Texture.SizeF.Height / Texture.Scale;

				if (c != ' ')
				{
					*vertices++ = Vertex2D(x + 0 - 1.f, y + h - 0.5f, color.ARGB, tx1, ty2);
					*vertices++ = Vertex2D(x + 0 - 1.f, y + 0 - 0.5f, color.ARGB, tx1, ty1);
					*vertices++ = Vertex2D(x + w - 1.f, y + h - 0.5f, color.ARGB, tx2, ty2);
					*vertices++ = Vertex2D(x + w - 1.f, y + 0 - 0.5f, color.ARGB, tx2, ty1);
					*vertices++ = Vertex2D(x + w - 1.f, y + h - 0.5f, color.ARGB, tx2, ty2);
					*vertices++ = Vertex2D(x + 0 - 1.f, y + 0 - 0.5f, color.ARGB, tx1, ty1);

					triangles += 2;

					if (triangles * 3 > maxVertices - 6)
					{
						vertexBuffer->Unlock();
						device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangles);
						vertexBuffer->Lock(0, 0, (BYTE**)&vertices, 0);
						triangles = 0;
					}
				}

				x += w - 2 * Texture.Space;
			}

			vertexBuffer->Unlock();

			if (triangles > 0)
			{
				device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangles);
			}

			device->ApplyStateBlock(backupStateBlock);
		}
		//---------------------------------------------------------------------------
		void FontDX8::PreReset()
		{
			Destroy();
		}
		//---------------------------------------------------------------------------
		void FontDX8::PostReset()
		{
			Initialize();
		}
		//---------------------------------------------------------------------------
	}
}