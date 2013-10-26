#ifndef OSHGUI_DRAWING_TEXTURE_HPP
#define OSHGUI_DRAWING_TEXTURE_HPP

#include "Size.hpp"
#include "Point.hpp"
#include "Rectangle.hpp"
#include "Vertex.hpp"
#include "../Misc/Strings.hpp"
#include <memory>

namespace OSHGui
{
	namespace Drawing
	{
		/**
		 * Die abstrakte Klasse repr�sentiert eine Textur und wird durch den Renderer erstellt.
		 */
		class Texture
		{
		public:
			/**
			 * Auflistung der m�glichen PixelFormate.
			 */
			enum class PixelFormat
			{
				RGB,
				RGBA,
				RGBA_4444,
				RGB_565,
				PVRTC2,
				PVRTC4,
				RGB_DXT1,
				RGBA_DXT1,
				RGBA_DXT3,
				RGBA_DXT5
			};

			/**
			 * Destructor der Klasse.
			 */
			virtual ~Texture()
			{

			}

			/**
			 * Ruft die (eventuell gepaddete) Gr��e der Textur in Pixeln ab.
			 *
			 * @return Gr��e der Textur
			 */
			virtual const SizeF& GetSize() const = 0;

			/**
			 * Ruft die Originalgr��e der Daten der Textur ab, mit denen sie erstellt wurde.
			 *
			 * @return Originalgr��e der Textur
			 */
			virtual const SizeF& GetOriginalDataSize() const = 0;

			/**
			 * Ruft die Pixel-zu-Texel Werte ab, die benutzt werden um Pixelwerte zu Texturkoordinaten umzurechnen.
			 *
			 * @return Pixel-zu-Texel Werte
			 */
			virtual const std::pair<float, float>& GetTexelScaling() const = 0;

			/**
			 * L�dt die Textur aus einer Datei.
			 *
			 * @param path Pfad zur Datei
			 */
			virtual void LoadFromFile(const Misc::AnsiString &path) = 0;

			/**
			 * L�dt die Textur aus dem Speicher. Die Daten im Buffer m�ssen im angegebenen Pixelformat vorliegen.
			 *
			 * @param buffer
			 * @param size die Gr��e des Bildes
			 * @param format
			 */
			virtual void LoadFromMemory(const void *buffer, const SizeF &size, PixelFormat format) = 0;

			/**
			 * Pr�ft, ob Texturen im angegebenen Format erstellt werden k�nnen.
			 *
			 * @param format das PixelFormat
			 * @return true falls das Format unterst�tzt wird
			 */
			virtual bool IsPixelFormatSupported(const PixelFormat format) const = 0;
		};

		typedef std::shared_ptr<Texture> TexturePtr;
	}
}

#endif