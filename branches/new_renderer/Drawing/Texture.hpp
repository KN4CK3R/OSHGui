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
		/*!
		\brief
			Abstract base class specifying the required interface for Texture objects.

			Texture objects are created via the Renderer.  The actual inner workings of
			any Texture object are dependant upon the Renderer (and underlying API) in
			use.  This base class defines the minimal set of functions that is required
			for the rest of the system to work.  Texture objects are only created
			through the Renderer object's texture creation functions.
		*/
		class Texture
		{
		public:
			/*!
			\brief
				Enumerated type containing the supported pixel formats that can be
				passed to loadFromMemory
			*/
			enum class PixelFormat
			{
				//! Each pixel is 3 bytes. RGB in that order.
				RGB,
				//! Each pixel is 4 bytes. RGBA in that order.
				RGBA,
				//! Each pixel is 2 bytes. RGBA in that order.
				RGBA_4444,
				//! Each pixel is 2 bytes. RGB in that order.
				RGB_565,
				//! PVR texture compression. Each pixel is 2 bits.
				PVRTC2,
				//! PVR texture compression. Each pixel is 4 bits.
				PVRTC4,
				//! S3 DXT1 texture compression (RGB).
				RGB_DXT1,
				//! S3 DXT1 texture compression (RGBA).
				RGBA_DXT1,
				//! S3 DXT1 texture compression (RGBA).
				RGBA_DXT3,
				//! S3 DXT1 texture compression (RGBA).
				RGBA_DXT5
			};

			/*!
			\brief
				Destructor for Texture base class.
			*/
			virtual ~Texture()
			{

			}

			/*!
			\brief
				Returns the current pixel size of the texture.

			\return
				Reference to a SizeF object that describes the size of the texture in
				pixels.
			*/
			virtual const SizeF& GetSize() const = 0;

			/*!
			\brief
				Returns the original pixel size of the data loaded into the texture.

			\return
				reference to a SizeF object that describes the original size, in pixels,
				of the data loaded into the texture.
			*/
			virtual const SizeF& GetOriginalDataSize() const = 0;

			/*!
			\brief
				Returns pixel to texel scale values that should be used for converting
				pixel values to texture co-ords.

			\return
				Reference to a Vector2 object that describes the scaling values required
				to accurately map pixel positions to texture co-ordinates.
			*/
			virtual const std::pair<float, float>& GetTexelScaling() const = 0;

			/*!
			\brief
				Loads the specified image file into the texture.  The texture is resized
				as required to hold the image.

			\param filename
				The filename of the image file that is to be loaded into the texture

			\param resourceGroup
				Resource group identifier to be passed to the resource provider when
				loading the image file.

			\return
				Nothing.
			*/
			virtual void LoadFromFile(const Misc::AnsiString& filename) = 0;

			virtual void LoadFromMemory(const void* buffer, const SizeF& buffer_size, PixelFormat pixel_format) = 0;

			/*!
			\brief
				Return whether the specified pixel format is supported by the system for
				the CEGUI::Texture implementation.

				The result of this call will vary according to the implementaion API
				and the capabilities of the hardware.

			\note
				Whether the CEGUI system as a whole will make use of support for any
				given pixel format will depend upon that format being recognised and
				supported by both the renderer module implementation and the ImageCodec
				module that is used to load texture data.

			\param fmt
				One of the PixelFormat enumerated values specifying the pixel format
				that is to be tested.

			\return
				- true if the specified PixelFormat is supported.
				- false if the specified PixelFormat is not supported.
			*/
			virtual bool IsPixelFormatSupported(const PixelFormat format) const = 0;
		};

		typedef std::shared_ptr<Texture> TexturePtr;
	}
}

#endif