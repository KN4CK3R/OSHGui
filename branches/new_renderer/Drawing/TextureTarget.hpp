#ifndef OSHGUI_TEXTURETARGET_HPP
#define OSHGUI_TEXTURETARGET_HPP

#include "RenderTarget.hpp"

namespace OSHGui
{
	namespace Drawing
	{
		/*!
		\brief
			Specialisation of RenderTarget interface that should be used as the base
			class for RenderTargets that are implemented using textures.
		*/
		class OSHGUI_EXPORT TextureTarget : public RenderTarget
		{
		public:
			/*!
			\brief
				Clear the surface of the underlying texture.
			*/
			virtual void Clear() = 0;

			/*!
			\brief
				Return a pointer to the CEGUI::Texture that the TextureTarget is using.

			\return
				Texture object that the TextureTarget uses when rendering imagery.
			*/
			virtual TexturePtr GetTexture() const = 0;

			/*!
			\brief
				Used to declare to the TextureTarget the largest size, in pixels, of the
				next set of incoming rendering operations.

			\note
				The main purpose of this is to allow for the implemenatation to resize
				the underlying texture so that it can hold the imagery that will be
				drawn.

			\param sz
				SizeF object describing the largest area that will be rendererd in the
				next batch of rendering operations.

			\exception InvalidRequestException
				May be thrown if the TextureTarget would not be able to handle the
				operations rendering content of the given size.
			*/
			virtual void DeclareRenderSize(const SizeF& sz) = 0;

			/*!
			\brief
				Return whether rendering done on the target texture is inverted in
				relation to regular textures.

				This is intended to be used when generating geometry for rendering the
				TextureTarget onto another surface.

			\return
				- true if the texture content should be considered as inverted
				vertically in comparison with other regular textures.
				- false if the texture content has the same orientation as regular
				textures.
			*/
			virtual bool IsRenderingInverted() const = 0;
		};

		typedef std::shared_ptr<TextureTarget> TextureTargetPtr;
	}
}

#endif