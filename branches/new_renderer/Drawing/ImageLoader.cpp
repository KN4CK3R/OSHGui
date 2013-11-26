/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "ImageLoader.hpp"
#include "../Misc/Exceptions.hpp"
#include "FreeImage.h"

namespace OSHGui
{
	namespace Drawing
	{
		ImageData LoadImageFromContainerToRGBABuffer(Misc::RawDataContainer &data)
		{
			auto stream = FreeImage_OpenMemory(data.GetDataPointer(), data.GetSize());

			auto format = FreeImage_GetFileTypeFromMemory(stream);
			if (format == FIF_UNKNOWN)
			{
				throw Misc::Exception();
			}

			auto image = FreeImage_LoadFromMemory(format, stream);
			if (image == nullptr)
			{
				throw Misc::Exception();
			}

			FreeImage_FlipVertical(image);

			auto temp = image;
			image = FreeImage_ConvertTo32Bits(image);
			FreeImage_Unload(temp);

			auto width = FreeImage_GetWidth(image);
			auto height = FreeImage_GetHeight(image);

			ImageData imageData;
			imageData.Data.resize(width * height * 4);
			imageData.Size = SizeF(width, height);

			auto pixeles = FreeImage_GetBits(image);
			for (int i = 0; i < width * height; ++i)
			{
				imageData.Data[i * 4 + 0] = pixeles[i * 4 + 2];
				imageData.Data[i * 4 + 1] = pixeles[i * 4 + 1];
				imageData.Data[i * 4 + 2] = pixeles[i * 4 + 0];
				imageData.Data[i * 4 + 3] = pixeles[i * 4 + 3];
			}

			FreeImage_Unload(image);

			FreeImage_CloseMemory(stream);

			return imageData;
		}
		//---------------------------------------------------------------------------
		ImageData LoadImageFromFileToRGBABuffer(const Misc::AnsiString &file)
		{
			Misc::RawDataContainer data;
			data.LoadFromFile(file);
			return LoadImageFromContainerToRGBABuffer(data);
		}
		//---------------------------------------------------------------------------
	}
}
