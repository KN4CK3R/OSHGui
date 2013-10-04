#include "Graphics.hpp"
#include <algorithm>

namespace OSHGui
{
	namespace Drawing
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Graphics::Graphics(std::shared_ptr<ITexture> &texture)
			: texture(texture)
		{
			texture->BeginUpdate();
		}
		//---------------------------------------------------------------------------
		Graphics::~Graphics()
		{
			texture->EndUpdate();
		}
		//---------------------------------------------------------------------------
		//Runtime Functions
		//---------------------------------------------------------------------------
		void Graphics::Clear()
		{
			texture->Clear();
		}
		//---------------------------------------------------------------------------
		void Graphics::SetPixel(const Point &origin, Color color)
		{
			SetPixel(origin.X, origin.Y, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::SetPixel(int x, int y, Color color)
		{
			texture->Fill(x, y, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillRectangle(const Point &origin, const Size &size, Color color)
		{
			FillRectangle(origin.X, origin.Y, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillRectangle(int x, int y, int w, int h, Color color)
		{
			texture->Fill(w < 0 ? x + w : x, h < 0 ? y + h : y, std::abs(w), std::abs(h), color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillRectangleGradient(const Point &origin, const Size &size, Color from, Color to, bool updown)
		{
			FillRectangleGradient(origin.X, origin.Y, size.Width, size.Height, from, to, updown);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillRectangleGradient(int x, int y, int w, int h, Color from, Color to, bool updown)
		{
			texture->FillGradient(w < 0 ? x + w : x, h < 0 ? y + h : y, std::abs(w), std::abs(h), from, to, updown);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillPolygon(const std::vector<Point> &vertices, Color color)
		{
			if (vertices.empty())
			{
				return;
			}

			std::vector<int> nodes;

			auto size = texture->GetSize();

			for (int y = 0; y < size.Height; ++y)
			{
				nodes.clear();
				int j = vertices.size() - 1;
				for (int i = 0; i < vertices.size(); ++i)
				{
					if ((vertices[i].Y < y && vertices[j].Y >= y) || (vertices[j].Y < y && vertices[i].Y >= y))
					{
						nodes.push_back((int)(vertices[i].X + (y - vertices[i].Y) / (float)(vertices[j].Y - vertices[i].Y) * (vertices[j].X - vertices[i].X)));
					}
					j = i;
				}

				if (!nodes.empty())
				{
					std::sort(std::begin(nodes), std::end(nodes));

					for (int i = 0; i < nodes.size() - 1; i += 2)
					{
						if (nodes[i] >= size.Width)
						{
							break;
						}
						if (nodes[i + 1] > 0)
						{
							if (nodes[i] < 0)
							{
								nodes[i] = 0;
							}
							if (nodes[i + 1] > size.Width)
							{
								nodes[i + 1] = size.Width;
							}
							if (nodes[i] < nodes[i + 1])
							{
								texture->Fill(nodes[i], y, nodes[i + 1] - nodes[i], 1, color);
							}
						}
					}
				}
			}
		}
		//---------------------------------------------------------------------------
		void Graphics::FillCircle(const Point &origin, int radius, Color color)
		{
			FillCircle(origin.X, origin.Y, radius, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillCircle(int x, int y, int radius, Color color)
		{
			FillEllipse(x, y, radius, radius, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillEllipse(const Point &origin, const Size &size, Color color)
		{
			FillEllipse(origin.X, origin.Y, size.Width, size.Height, color);
		}
		//---------------------------------------------------------------------------
		void Graphics::FillEllipse(int centerX, int centerY, int width, int height, Color color)
		{
			int a = width / 2;
			int b = height / 2;
			int xc = centerX;
			int yc = centerY;
			int x = 0;
			int y = b;
			int a2 = a * a;
			int b2 = b * b;
			int xp = 1;
			int yp = y;

			while (b2 * x < a2 * y)
			{
				++x;
				if ((b2 * x * x + a2 * (y - 0.5f) * (y - 0.5f) - a2 * b2) >= 0)
				{
					y--;
				}
				if (x == 1 && y != yp)
				{
					SetPixel(xc, yc + yp - 1, color);
					SetPixel(xc, yc - yp, color);
				}
				if (y != yp)
				{
					FillRectangle(xc - x + 1, yc - yp, 2 * x - 1, 1, color);
					FillRectangle(xc - x + 1, yc + yp, 2 * x - 1, 1, color);
					yp = y;
					xp = x;
				}

				if (b2 * x >= a2 * y)
				{
					FillRectangle(xc - x, yc - yp, 2 * x + 1, 1, color);
					FillRectangle(xc - x, yc + yp, 2 * x + 1, 1, color);
				}
			}

			xp = x;
			yp = y;
			int divHeight = 1;

			while (y != 0)
			{
				y--;
				if ((b2 * (x + 0.5f) * (x + 0.5f) + a2 * y * y - a2 * b2) <= 0)
				{
					x++;
				}

				if (x != xp)
				{
					divHeight = yp - y;

					FillRectangle(xc - xp, yc - yp, 2 * xp + 1, divHeight, color);
					FillRectangle(xc - xp, yc + y + 1, 2 * xp + 1, divHeight, color);

					xp = x;
					yp = y;
				}

				if (y == 0)
				{
					divHeight = yp - y + 1;

					FillRectangle(xc - xp, yc - yp, 2 * x + 1, divHeight, color);
					FillRectangle(xc - xp, yc + y, 2 * x + 1, divHeight, color);
				}
			}
		}
		//---------------------------------------------------------------------------
	}
}