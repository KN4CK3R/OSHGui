/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TextureDX10.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		TextureDX10::TextureDX10(IDirect3DDevice10 *device)
		{
			this->device = device;
			texture = NULL;
			lock.pData = NULL;
		}
		//---------------------------------------------------------------------------
		TextureDX10::~TextureDX10()
		{
			SAFE_RELEASE(texture);
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		ID3D10Texture2D* TextureDX10::GetTexture()
		{
			return texture;
		}
		//---------------------------------------------------------------------------
		bool TextureDX10::IsLocked()
		{
			return lock.pData != NULL;
		}
		//---------------------------------------------------------------------------
		//Runtime-Functions
		//---------------------------------------------------------------------------
		bool TextureDX10::Create(const SizeF &size)
		{
			SAFE_RELEASE(texture);
			
			D3D10_TEXTURE2D_DESC textureDesc;
			memset((void*)&textureDesc, 0x00, sizeof(D3D10_TEXTURE2D_DESC));
			textureDesc.Width = size.Width;
			textureDesc.Height = size.Height;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D10_USAGE_DYNAMIC;
			textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE|D3D10_CPU_ACCESS_READ;
			
			if (!FAILED(device->CreateTexture2D(&textureDesc, NULL, &texture)))
			{
				this->size = size;
				return true;
			}
			return false;
		}
		//---------------------------------------------------------------------------
		void TextureDX10::BeginUpdate()
		{
			texture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ_WRITE, 0, &lock);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::EndUpdate()
		{
			texture->Unmap(D3D10CalcSubresource(0, 0, 1));
			lock.pData = NULL;
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Clear()
		{
			Fill(0, 0, size.Width, size.Height, 0x00000000);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Clear(int x, int y)
		{
			Fill(x, y, 1, 1, 0x00000000);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Clear(const Drawing::PointF &point)
		{
			Fill(point.X, point.Y, 1, 1, 0x00000000);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Clear(Drawing::RectangleF &rect)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), 0x00000000);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Clear(int x, int y, int w, int h)
		{
			Fill(x, y, w, h, 0x00000000);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Fill(D3DCOLOR color)
		{
			Fill(0, 0, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Fill(int x, int y, D3DCOLOR color)
		{
			Fill(x, y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Fill(const Drawing::PointF &point, D3DCOLOR color)
		{
			Fill(point.X, point.Y, 1, 1, color);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Fill(Drawing::RectangleF &rect, D3DCOLOR color)
		{
			Fill(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), color);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Fill(int _x, int _y, int _w, int _h, D3DCOLOR _color)
		{
			BYTE color[4] = { (BYTE)RED(_color), (BYTE)GREEN(_color), (BYTE)BLUE(_color), (BYTE)ALPHA(_color) };
		
			BYTE *raw = (BYTE*)lock.pData;
			for (int y = 0; y < _h; y++)
			{
				int row = (_y + y) * lock.RowPitch;
				
				for (int x = 0; x < _w; x++)
				{
					int index = (_x + x) * 4 + row;
					
					raw[index] = color[0];
					raw[index + 1] = color[1];
					raw[index + 2] = color[2];
					raw[index + 3] = color[3];
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX10::FillGradient(D3DCOLOR from, D3DCOLOR to, bool updown)
		{
			FillGradient(0, 0, size.Width, size.Height, from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::FillGradient(Drawing::RectangleF &rect, D3DCOLOR from, D3DCOLOR to, bool updown)
		{
			FillGradient(rect.GetLeft(), rect.GetTop(), rect.GetWidth(), rect.GetHeight(), from, to, updown);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::FillGradient(int _x, int _y, int _w, int _h, D3DCOLOR from, D3DCOLOR to, bool updown)
		{
			BYTE color[4] = { (BYTE)RED(from), (BYTE)GREEN(from), (BYTE)BLUE(from), (BYTE)ALPHA(from) };
			float step[4];
		
			BYTE *raw = (BYTE*)lock.pData;
			
			if (updown)
			{
				step[0] = ((BYTE)RED(to) - color[0]) / (float)(_h - 1);
				step[1] = ((BYTE)GREEN(to) - color[1]) / (float)(_h - 1);
				step[2] = ((BYTE)BLUE(to) - color[2]) / (float)(_h - 1);
				step[3] = ((BYTE)ALPHA(to) - color[3]) / (float)(_h - 1);
				
				for (int y = 0; y < _h; y++)
				{
					int row = (_y + y) * lock.RowPitch;
					
					for (int x = 0; x < _w; x++)
					{
						int index = (_x + x) * 4 + row;
						
						raw[index] = color[0] + (BYTE)(y * step[0]);
						raw[index + 1] = color[1] + (BYTE)(y * step[1]);
						raw[index + 2] = color[2] + (BYTE)(y * step[2]);
						raw[index + 3] = color[3] + (BYTE)(y * step[3]);
					}
				}
			}
			else
			{
				step[0] = ((BYTE)RED(to) - color[0]) / (float)(_w - 1);
				step[1] = ((BYTE)GREEN(to) - color[1]) / (float)(_w - 1);
				step[2] = ((BYTE)BLUE(to) - color[2]) / (float)(_w - 1);
				step[3] = ((BYTE)ALPHA(to) - color[3]) / (float)(_w - 1);
			
				for (int y = 0; y < _h; y++)
				{
					int row = (_y + y) * lock.RowPitch;
					
					for (int x = 0; x < _w; x++)
					{
						int index = (_x + x) * 4 + row;
						
						raw[index] = color[0] + (BYTE)(x * step[0]);
						raw[index + 1] = color[1] + (BYTE)(x * step[1]);
						raw[index + 2] = color[2] + (BYTE)(x * step[2]);
						raw[index + 3] = color[3] + (BYTE)(x * step[3]);
					}
				}
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Rotate(int degrees)
		{
			if (!(degrees == 90 || degrees == 180 || degrees == 270))
			{
				return;
			}
		
			ID3D10Texture2D *temp;
			D3D10_MAPPED_TEXTURE2D tempLock;
			Drawing::SizeF tempSize = size, bckSize = size;

			if (degrees == 90 || degrees == 270)
			{
				int w = tempSize.Width;
				tempSize.Width = tempSize.Height;
				tempSize.Height = w;
			}
			
			D3D10_TEXTURE2D_DESC textureDesc;
			memset((void*)&textureDesc, 0x00, sizeof(D3D10_TEXTURE2D_DESC));
			textureDesc.Width = tempSize.Width;
			textureDesc.Height = tempSize.Height;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.Usage = D3D10_USAGE_DYNAMIC;
			textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE;
			textureDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE|D3D10_CPU_ACCESS_READ;
			
			if (!FAILED(device->CreateTexture2D(&textureDesc, NULL, &temp)))
			{
				size = tempSize;
				tempSize.Inflate(-1, -1);

				temp->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ_WRITE, 0, &tempLock);
			
				BYTE *raw = (BYTE*)lock.pData;
				BYTE *tempRaw = (BYTE*)tempLock.pData;
				
				if (degrees == 90)
				{
					for (int y = 0; y < bckSize.Height; y++)
					{
						int row = y * lock.RowPitch,
							tempRow = (tempSize.Width - y) * 4;
							
						for (int x = 0; x < bckSize.Width; x++)
						{
							int index = x * 4 + row,
								tempIndex = tempRow + x * tempLock.RowPitch;
								
							tempRaw[tempIndex] = raw[index];
							tempRaw[tempIndex + 1] = raw[index + 1];
							tempRaw[tempIndex + 2] = raw[index + 2];
							tempRaw[tempIndex + 3] = raw[index + 3];
						}
					}
				}
				else if (degrees == 180)
				{
					for (int y = 0; y < bckSize.Height; y++)
					{
						int row = y * lock.RowPitch,
							tempRow = (tempSize.Height - y) * tempLock.RowPitch;
							
						for (int x = 0; x < bckSize.Width; x++)
						{
							int index = x * 4 + row,
								tempIndex = (tempSize.Width - x) * 4 + tempRow;
								
							tempRaw[tempIndex] = raw[index];
							tempRaw[tempIndex + 1] = raw[index + 1];
							tempRaw[tempIndex + 2] = raw[index + 2];
							tempRaw[tempIndex + 3] = raw[index + 3];
						}
					}
				}
				else if (degrees == 270)
				{
					for (int y = 0; y < bckSize.Height; y++)
					{
						int row = y * lock.RowPitch,
							tempRow = y * 4;
							
						for (int x = 0; x < bckSize.Width; x++)
						{
							int index = x * 4 + row,
								tempIndex = tempRow + (tempSize.Width - x) * tempLock.RowPitch;
								
							tempRaw[tempIndex] = raw[index];
							tempRaw[tempIndex + 1] = raw[index + 1];
							tempRaw[tempIndex + 2] = raw[index + 2];
							tempRaw[tempIndex + 3] = raw[index + 3];
						}
					}
				}
				
				texture->Unmap(D3D10CalcSubresource(0, 0, 1));
				temp->Unmap(D3D10CalcSubresource(0, 0, 1));
				SAFE_RELEASE(texture);
				
				texture = temp;
				texture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ_WRITE, 0, &lock);
			}
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Insert(const Drawing::PointF &point, Drawing::TextureDX10 *texture)
		{
			Insert(point.X, point.Y, texture);
		}
		//---------------------------------------------------------------------------
		void TextureDX10::Insert(int _x, int _y, Drawing::ITexture *texture)
		{
			if (texture == NULL || texture->IsLocked())
			{
				return;
			}
		
			ID3D10Texture2D *temp = ((TextureDX10*)texture)->GetTexture();
			
			if (temp == NULL)
			{
				return;
			}
			
			Drawing::SizeF tempSize = texture->GetSize();
			
			if (tempSize.Width + _x > size.Width || tempSize.Height + _y > size.Height)
			{
				return;
			}
			
			D3D10_MAPPED_TEXTURE2D tempLock;
			temp->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_READ_WRITE, 0, &tempLock);
		
			BYTE *raw = (BYTE*)lock.pData;
			BYTE *tempRaw = (BYTE*)tempLock.pData;
				
			for (int y = 0; y < tempSize.Height; y++)
			{
				int row = (y + _y) * lock.RowPitch,
					tempRow = y * tempLock.RowPitch;
			
				for (int x = 0; x < tempSize.Width; x++)
				{
					int index = (x + _x) * 4 + row,
						tempIndex = x * 4 + tempRow;
						
					raw[index] = tempRaw[tempIndex];
					raw[index + 1] = tempRaw[tempIndex + 1];
					raw[index + 2] = tempRaw[tempIndex + 2];
					raw[index + 3] = tempRaw[tempIndex + 3];
				}
			}
			
			temp->Unmap(D3D10CalcSubresource(0, 0, 1));
		}
		//---------------------------------------------------------------------------
	}
}