/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "RendererDX8.hpp"
#define NOMINMAX
#include <d3d8.h>
#pragma comment(lib, "d3d8.lib")

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

				auto D3DXMatrixTransformation = [](D3DXMATRIX *pout, const D3DXVECTOR3 *pscalingcenter, const D3DXQUATERNION *pscalingrotation, const D3DXVECTOR3 *pscaling, const D3DXVECTOR3 *protationcenter, const D3DXQUATERNION *protation, const D3DXVECTOR3 *ptranslation) -> D3DXMATRIX*
				{
					D3DXMATRIX m1, m2, m3, m4, m5, m6, m7;
					D3DXQUATERNION prc;
					D3DXVECTOR3 psc, pt;

					if (!pscalingcenter)
					{
						psc.x = 0.0f;
						psc.y = 0.0f;
						psc.z = 0.0f;
					}
					else
					{
						psc.x = pscalingcenter->x;
						psc.y = pscalingcenter->y;
						psc.z = pscalingcenter->z;
					}

					if (!protationcenter)
					{
						prc.x = 0.0f;
						prc.y = 0.0f;
						prc.z = 0.0f;
					}
					else
					{
						prc.x = protationcenter->x;
						prc.y = protationcenter->y;
						prc.z = protationcenter->z;
					}

					if (!ptranslation)
					{
						pt.x = 0.0f;
						pt.y = 0.0f;
						pt.z = 0.0f;
					}
					else
					{
						pt.x = ptranslation->x;
						pt.y = ptranslation->y;
						pt.z = ptranslation->z;
					}

					auto D3DXMatrixIdentity = [](D3DXMATRIX *pOut) -> D3DXMATRIX*
					{
						pOut->m[0][1] = pOut->m[0][2] = pOut->m[0][3] =
						pOut->m[1][0] = pOut->m[1][2] = pOut->m[1][3] =
						pOut->m[2][0] = pOut->m[2][1] = pOut->m[2][3] =
						pOut->m[3][0] = pOut->m[3][1] = pOut->m[3][2] = 0.0f;

						pOut->m[0][0] = pOut->m[1][1] = pOut->m[2][2] = pOut->m[3][3] = 1.0f;
						return pOut;
					};

					auto D3DXMatrixTranslation = [D3DXMatrixIdentity](D3DXMATRIX *pout, FLOAT x, FLOAT y, FLOAT z) -> D3DXMATRIX*
					{
						D3DXMatrixIdentity(pout);
						pout->m[3][0] = x;
						pout->m[3][1] = y;
						pout->m[3][2] = z;
						return pout;
					};

					D3DXMatrixTranslation(&m1, -psc.x, -psc.y, -psc.z);

					auto D3DXMatrixRotationQuaternion = [D3DXMatrixIdentity](D3DXMATRIX *pout, const D3DXQUATERNION *pq) -> D3DXMATRIX*
					{
						D3DXMatrixIdentity(pout);
						pout->m[0][0] = 1.0f - 2.0f * (pq->y * pq->y + pq->z * pq->z);
						pout->m[0][1] = 2.0f * (pq->x *pq->y + pq->z * pq->w);
						pout->m[0][2] = 2.0f * (pq->x * pq->z - pq->y * pq->w);
						pout->m[1][0] = 2.0f * (pq->x * pq->y - pq->z * pq->w);
						pout->m[1][1] = 1.0f - 2.0f * (pq->x * pq->x + pq->z * pq->z);
						pout->m[1][2] = 2.0f * (pq->y *pq->z + pq->x *pq->w);
						pout->m[2][0] = 2.0f * (pq->x * pq->z + pq->y * pq->w);
						pout->m[2][1] = 2.0f * (pq->y *pq->z - pq->x *pq->w);
						pout->m[2][2] = 1.0f - 2.0f * (pq->x * pq->x + pq->y * pq->y);
						return pout;
					};

					auto D3DXMatrixInverse = [](D3DXMATRIX *pout, FLOAT *pdeterminant, const D3DXMATRIX *pm) -> D3DXMATRIX*
					{
						D3DXMATRIX out;
						D3DXVECTOR4 v, vec[3];

						auto D3DXVec4Cross = [](D3DXVECTOR4 *pout, const D3DXVECTOR4 *pv1, const D3DXVECTOR4 *pv2, const D3DXVECTOR4 *pv3) -> D3DXVECTOR4*
						{
							D3DXVECTOR4 out;
							out.x = pv1->y * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->y * pv3->w - pv3->y * pv2->w) + pv1->w * (pv2->y * pv3->z - pv2->z *pv3->y);
							out.y = -(pv1->x * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->x * pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->z - pv3->x * pv2->z));
							out.z = pv1->x * (pv2->y * pv3->w - pv3->y * pv2->w) - pv1->y * (pv2->x *pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->y - pv3->x * pv2->y);
							out.w = -(pv1->x * (pv2->y * pv3->z - pv3->y * pv2->z) - pv1->y * (pv2->x * pv3->z - pv3->x *pv2->z) + pv1->z * (pv2->x * pv3->y - pv3->x * pv2->y));
							*pout = out;
							return pout;
						};

						auto D3DXMatrixDeterminant = [D3DXVec4Cross](const D3DXMATRIX *pm) -> float
						{
							D3DXVECTOR4 minor, v1, v2, v3;

							v1.x = pm->m[0][0]; v1.y = pm->m[1][0]; v1.z = pm->m[2][0]; v1.w = pm->m[3][0];
							v2.x = pm->m[0][1]; v2.y = pm->m[1][1]; v2.z = pm->m[2][1]; v2.w = pm->m[3][1];
							v3.x = pm->m[0][2]; v3.y = pm->m[1][2]; v3.z = pm->m[2][2]; v3.w = pm->m[3][2];
							D3DXVec4Cross(&minor, &v1, &v2, &v3);
							float det =  - (pm->m[0][3] * minor.x + pm->m[1][3] * minor.y + pm->m[2][3] * minor.z + pm->m[3][3] * minor.w);
							return det;
						};

						float det = D3DXMatrixDeterminant(pm);
						if (!det) return nullptr;
						if (pdeterminant) *pdeterminant = det;
						for (int i = 0; i < 4; ++i)
						{
							for (int j = 0; j < 4; ++j)
							{
								if (j != i )
								{
									int a = j;
									if (j > i) a = a-1;
									vec[a].x = pm->m[j][0];
									vec[a].y = pm->m[j][1];
									vec[a].z = pm->m[j][2];
									vec[a].w = pm->m[j][3];
								}
							}
							D3DXVec4Cross(&v, &vec[0], &vec[1], &vec[2]);
							out.m[0][i] = pow(-1.0f, i) * v.x / det;
							out.m[1][i] = pow(-1.0f, i) * v.y / det;
							out.m[2][i] = pow(-1.0f, i) * v.z / det;
							out.m[3][i] = pow(-1.0f, i) * v.w / det;
						}

						*pout = out;
						return pout;
					};

					if (!pscalingrotation)
					{
						D3DXMatrixIdentity(&m2);
						D3DXMatrixIdentity(&m4);
					}
					else
					{
						D3DXMatrixRotationQuaternion(&m4, pscalingrotation);
						D3DXMatrixInverse(&m2, nullptr, &m4);
					}

					auto D3DXMatrixScaling = [D3DXMatrixIdentity](D3DXMATRIX *pout, float sx, float sy, float sz) -> D3DXMATRIX*
					{
						D3DXMatrixIdentity(pout);
						pout->m[0][0] = sx;
						pout->m[1][1] = sy;
						pout->m[2][2] = sz;
						return pout;
					};

					if (!pscaling) D3DXMatrixIdentity(&m3);
					else D3DXMatrixScaling(&m3, pscaling->x, pscaling->y, pscaling->z);

					if (!protation) D3DXMatrixIdentity(&m6);
					else D3DXMatrixRotationQuaternion(&m6, protation);

					D3DXMatrixTranslation(&m5, psc.x - prc.x,  psc.y - prc.y,  psc.z - prc.z);
					D3DXMatrixTranslation(&m7, prc.x + pt.x, prc.y + pt.y, prc.z + pt.z);

					auto D3DXMatrixMultiply = [](D3DXMATRIX *pout, const D3DXMATRIX *pm1, const D3DXMATRIX *pm2) -> D3DXMATRIX*
					{
						D3DXMATRIX out;
						for (int i = 0; i < 4; ++i)
						{
							for (int j = 0; j < 4; ++j)
							{
								out.m[i][j] = pm1->m[i][0] * pm2->m[0][j] + pm1->m[i][1] * pm2->m[1][j] + pm1->m[i][2] * pm2->m[2][j] + pm1->m[i][3] * pm2->m[3][j];
							}
						}
						*pout = out;
						return pout;
					};

					D3DXMatrixMultiply(&m1, &m1, &m2);
					D3DXMatrixMultiply(&m1, &m1, &m3);
					D3DXMatrixMultiply(&m1, &m1, &m4);
					D3DXMatrixMultiply(&m1, &m1, &m5);
					D3DXMatrixMultiply(&m1, &m1, &m6);
					D3DXMatrixMultiply(pout, &m1, &m7);
					return pout;
				};

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

			auto D3DXVec3TransformCoordArray = [](D3DXVECTOR3* out, UINT outstride, const D3DXVECTOR3* in, UINT instride, const D3DXMATRIX* matrix, UINT elements) -> D3DXVECTOR3*
			{
				auto D3DXVec3TransformCoord = [](D3DXVECTOR3 *pout, const D3DXVECTOR3 *pv, const D3DXMATRIX *pm) -> D3DXVECTOR3*
				{
					D3DXVECTOR3 out;
					FLOAT norm;

					norm = pm->m[0][3] * pv->x + pm->m[1][3] * pv->y + pm->m[2][3] *pv->z + pm->m[3][3];

					out.x = (pm->m[0][0] * pv->x + pm->m[1][0] * pv->y + pm->m[2][0] * pv->z + pm->m[3][0]) / norm;
					out.y = (pm->m[0][1] * pv->x + pm->m[1][1] * pv->y + pm->m[2][1] * pv->z + pm->m[3][1]) / norm;
					out.z = (pm->m[0][2] * pv->x + pm->m[1][2] * pv->y + pm->m[2][2] * pv->z + pm->m[3][2]) / norm;

					*pout = out;

					return pout;
				};

				for (auto i = 0; i < elements; ++i)
				{
					D3DXVec3TransformCoord((D3DXVECTOR3*)((char*)out + outstride * i), (const D3DXVECTOR3*)((const char*)in + instride * i), matrix);
				}
				return out;
			};

			D3DXVec3TransformCoordArray((D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), (D3DXVECTOR3*)&vertices[0].x, sizeof(Vertex2D), &matrix, 6);

			device->SetTexture(0, std::static_pointer_cast<TextureDX8>(texture)->GetTexture());
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