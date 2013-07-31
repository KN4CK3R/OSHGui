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
			device->CreateStateBlock(D3DSBT_ALL, &stateBlockHandle);
		}
		//---------------------------------------------------------------------------
		void RendererDX8::Begin()
		{
			device->CaptureStateBlock(stateBlockHandle);

			device->SetTexture(0, nullptr);
			device->SetPixelShader(0);
			device->SetVertexShader(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1);

			device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			device->SetRenderState(D3DRS_ALPHAREF, 0x00);
			device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			device->SetRenderState(D3DRS_CLIPPING, TRUE);
			device->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
			device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_RED);
			device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
			device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			device->SetRenderState(D3DRS_FOGENABLE, FALSE);
			device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
			device->SetRenderState(D3DRS_LIGHTING, FALSE);
			device->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
			device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
			device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
			device->SetRenderState(D3DRS_VERTEXBLEND, FALSE);
			device->SetRenderState(D3DRS_WRAP0, 0);

			device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
			device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			device->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
			
			SetRenderRectangle(Drawing::Rectangle(GetRenderDimension()));
		}
		//---------------------------------------------------------------------------
		void RendererDX8::End()
		{
			Flush();

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
				device->SetTexture(0, nullptr);
				device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, verticesNum / 3, vertices, sizeof(Vertex2D));
				
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

			Flush();

			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			D3DXVECTOR2 screenPosition(x, y);
			float scaleWidth = static_cast<float>(w) / texture->GetSize().Width;
			float scaleHeight = static_cast<float>(h) / texture->GetSize().Height;
			D3DXVECTOR2 scaling = scaleWidth < scaleHeight ? D3DXVECTOR2(scaleWidth, scaleWidth) : D3DXVECTOR2(scaleHeight, scaleHeight);

			auto D3DXMatrixTransformation2D = [](D3DXMATRIX *pout, const D3DXVECTOR2 *pscalingcenter, float scalingrotation, const D3DXVECTOR2 *pscaling, const D3DXVECTOR2 *protationcenter, float rotation, const D3DXVECTOR2 *ptranslation) -> D3DXMATRIX*
			{
				D3DXQUATERNION rot, sca_rot;
				D3DXVECTOR3 rot_center, sca, sca_center, trans;

				if (pscalingcenter)
				{
					sca_center.x = pscalingcenter->x;
					sca_center.y = pscalingcenter->y;
					sca_center.z = 0.0f;
				}
				else
				{
					sca_center.x = 0.0f;
					sca_center.y = 0.0f;
					sca_center.z = 0.0f;
				}

				if (pscaling)
				{
					sca.x = pscaling->x;
					sca.y = pscaling->y;
					sca.z = 1.0f;
				}
				else
				{
					sca.x = 1.0f;
					sca.y = 1.0f;
					sca.z = 1.0f;
				}

				if (protationcenter)
				{
					rot_center.x = protationcenter->x;
					rot_center.y = protationcenter->y;
					rot_center.z = 0.0f;
				}
				else
				{
					rot_center.x = 0.0f;
					rot_center.y = 0.0f;
					rot_center.z = 0.0f;
				}

				if (ptranslation)
				{
					trans.x = ptranslation->x;
					trans.y = ptranslation->y;
					trans.z = 0.0f;
				}
				else
				{
					trans.x = 0.0f;
					trans.y = 0.0f;
					trans.z = 0.0f;
				}

				rot.w = cos(rotation / 2.0f);
				rot.x = 0.0f;
				rot.y = 0.0f;
				rot.z = sin(rotation / 2.0f);

				sca_rot.w = cos(scalingrotation / 2.0f);
				sca_rot.x = 0.0f;
				sca_rot.y = 0.0f;
				sca_rot.z = sin(scalingrotation / 2.0f);

				D3DXMatrixTransformation(pout, &sca_center, &sca_rot, &sca, &rot_center, &rot, &trans);

				return pout;
			};

			D3DXMATRIX matrix;
			D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scaling, nullptr, 0.0f, &screenPosition);

			Vertex2D vertices[6] = {
				//x     y     z     color       u     v
				{ 0.0f, 0.0f, 0.0f, color.ARGB, 0.0f, 0.0f },
				{ w,    0.0f, 0.0f, color.ARGB, 1.0f, 0.0f },
				{ w,    h,    0.0f, color.ARGB, 1.0f, 1.0f },
				{ 0.0f, h,    0.0f, color.ARGB, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 0.0f, color.ARGB, 0.0f, 0.0f },
				{ w,    h,    0.0f, color.ARGB, 1.0f, 1.0f }
			};

			auto D3DXVec3TransformCoordArray = [](D3DXVECTOR3* out, UINT outstride, CONST D3DXVECTOR3* in, UINT instride, CONST D3DXMATRIX* matrix, UINT elements) -> D3DXVECTOR3*
			{
				for (auto i = 0; i < elements; ++i)
				{
					D3DXVec3TransformCoord((D3DXVECTOR3*)((char*)out + outstride * i), (CONST D3DXVECTOR3*)((const char*)in + instride * i), matrix);
				}
				return out;
			};


			D3DXVec3TransformCoordArray((D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), (D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), &matrix, 6);

			device->SetTexture(0, std::static_pointer_cast<TextureDX8>(texture)->GetTexture());
			device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(Vertex2D));
		}
		void RendererDX8::RenderTexture(IDirect3DTexture8 *texture, Size size, int x, int y, int w, int h)
		{
			Flush();
			
			x = x + renderRect.GetLeft();
			y = y + renderRect.GetTop();

			D3DXVECTOR2 screenPosition(x, y);
			float scaleWidth = static_cast<float>(w) / size.Width;
			float scaleHeight = static_cast<float>(h) / size.Height;
			D3DXVECTOR2 scaling = scaleWidth < scaleHeight ? D3DXVECTOR2(scaleWidth, scaleWidth) : D3DXVECTOR2(scaleHeight, scaleHeight);

			auto D3DXMatrixTransformation2D = [](D3DXMATRIX *pout, const D3DXVECTOR2 *pscalingcenter, float scalingrotation, const D3DXVECTOR2 *pscaling, const D3DXVECTOR2 *protationcenter, float rotation, const D3DXVECTOR2 *ptranslation) -> D3DXMATRIX*
			{
				D3DXQUATERNION rot, sca_rot;
				D3DXVECTOR3 rot_center, sca, sca_center, trans;

				if (pscalingcenter)
				{
					sca_center.x = pscalingcenter->x;
					sca_center.y = pscalingcenter->y;
					sca_center.z = 0.0f;
				}
				else
				{
					sca_center.x = 0.0f;
					sca_center.y = 0.0f;
					sca_center.z = 0.0f;
				}

				if (pscaling)
				{
					sca.x = pscaling->x;
					sca.y = pscaling->y;
					sca.z = 1.0f;
				}
				else
				{
					sca.x = 1.0f;
					sca.y = 1.0f;
					sca.z = 1.0f;
				}

				if (protationcenter)
				{
					rot_center.x = protationcenter->x;
					rot_center.y = protationcenter->y;
					rot_center.z = 0.0f;
				}
				else
				{
					rot_center.x = 0.0f;
					rot_center.y = 0.0f;
					rot_center.z = 0.0f;
				}

				if (ptranslation)
				{
					trans.x = ptranslation->x;
					trans.y = ptranslation->y;
					trans.z = 0.0f;
				}
				else
				{
					trans.x = 0.0f;
					trans.y = 0.0f;
					trans.z = 0.0f;
				}

				rot.w = cos(rotation / 2.0f);
				rot.x = 0.0f;
				rot.y = 0.0f;
				rot.z = sin(rotation / 2.0f);

				sca_rot.w = cos(scalingrotation / 2.0f);
				sca_rot.x = 0.0f;
				sca_rot.y = 0.0f;
				sca_rot.z = sin(scalingrotation / 2.0f);

				D3DXMatrixTransformation(pout, &sca_center, &sca_rot, &sca, &rot_center, &rot, &trans);

				return pout;
			};

			D3DXMATRIX matrix;
			D3DXMatrixTransformation2D(&matrix, nullptr, 0.0f, &scaling, nullptr, 0.0f, &screenPosition);

			Vertex2D vertices[6] = {
				//x     y     z     color       u     v
				{ 0.0f, 0.0f, 0.0f, color.ARGB, 0.0f, 0.0f },
				{ w,    0.0f, 0.0f, color.ARGB, 1.0f, 0.0f },
				{ w,    h,    0.0f, color.ARGB, 1.0f, 1.0f },
				{ 0.0f, h,    0.0f, color.ARGB, 0.0f, 1.0f },
				{ 0.0f, 0.0f, 0.0f, color.ARGB, 0.0f, 0.0f },
				{ w,    h,    0.0f, color.ARGB, 1.0f, 1.0f }
			};

			auto D3DXVec3TransformCoordArray = [](D3DXVECTOR3* out, UINT outstride, CONST D3DXVECTOR3* in, UINT instride, CONST D3DXMATRIX* matrix, UINT elements) -> D3DXVECTOR3*
			{
				for (auto i = 0; i < elements; ++i)
				{
					D3DXVec3TransformCoord((D3DXVECTOR3*)((char*)out + outstride * i), (CONST D3DXVECTOR3*)((const char*)in + instride * i), matrix);
				}
				return out;
			};


			D3DXVec3TransformCoordArray((D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), (D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), &matrix, 6);

			device->SetTexture(0, texture);
			device->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, vertices, sizeof(Vertex2D));
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
			
			std::static_pointer_cast<FontDX8>(font)->RenderText(x, y, color, text);

			/*RECT clip = { x, y, x + w, y + h };

			DWORD fvf;
			device->GetVertexShader(&fvf);
			std::static_pointer_cast<FontDX8>(font)->GetFont()->DrawTextA(text.c_str(), text.length(), &clip, DT_LEFT | DT_TOP, color.ARGB);
			device->SetVertexShader(fvf);*/
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
				device->DrawPrimitiveUP(D3DPT_LINELIST, verticesNum / 2, vertices, sizeof(Vertex2D));
				
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