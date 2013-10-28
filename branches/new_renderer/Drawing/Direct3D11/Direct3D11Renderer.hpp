/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_DRAWING_DIRECT3D11RENDERER_HPP
#define OSHGUI_DRAWING_DIRECT3D11RENDERER_HPP

#include "../Renderer.hpp"

#include <vector>
#include <map>

/*struct ID3D10Device;
struct ID3D10Effect;
struct ID3D10EffectTechnique;
struct ID3D10InputLayout;
struct ID3D10EffectShaderResourceVariable;
struct ID3D10EffectMatrixVariable;
struct ID3D10ShaderResourceView;
struct ID3D10EffectScalarVariable;
struct D3DXMATRIX;*/

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectShaderResourceVariable;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectScalarVariable;
struct ID3D11ShaderResourceView;

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

struct IDevice11
{
	IDevice11(ID3D11Device *device, ID3D11DeviceContext *context)
		: Device(device),
		  Context(context)
	{

	}

	ID3D11Device *Device;
	ID3D11DeviceContext *Context;
};

namespace OSHGui
{
	namespace Drawing
	{
		class Direct3D11Texture;
		class Direct3D11TextureTarget;
		class Direct3D11GeometryBuffer;

		/**
		 * Die Direct3D11 Variante des Renderer.
		 */
		class OSHGUI_EXPORT Direct3D11Renderer : public Renderer
		{
		public:
			/**
			 * Konstruktor der Klasse.
			 *
			 * \param device Direct3D11 Device
			 * \param context Direct3D11 Context
			 */
			Direct3D11Renderer(ID3D11Device *device, ID3D11DeviceContext *context);
			/**
			 * Destruktor der Klasse.
			 */
			~Direct3D11Renderer();

			/**
			 * Hilfsfunktion, die vor einem D3D Reset aufgerufen werden muss.
			 */
			void PreD3DReset();
			/**
			 * Hilfsfunktion, die nach einem D3D Reset aufgerufen werden muss.
			 */
			void PostD3DReset();

			/**
			 * Ruft das Direct3D11 Device ab.
			 */
			IDevice11& GetDevice();

			void BindTechniquePass(const BlendMode mode, const bool clipped);
			void SetCurrentTextureShaderResource(ID3D11ShaderResourceView* srv);
			void SetProjectionMatrix(D3DXMATRIX &matrix);
			void SetWorldMatrix(D3DXMATRIX &matrix);

			virtual RenderTargetPtr& GetDefaultRenderTarget() override;
			virtual GeometryBufferPtr CreateGeometryBuffer() override;
			virtual TextureTargetPtr CreateTextureTarget() override;
			virtual TexturePtr CreateTexture() override;
			virtual TexturePtr CreateTexture(const Misc::AnsiString &filename) override;
			virtual TexturePtr CreateTexture(const SizeF &size) override;

			virtual void BeginRendering() override;
			virtual void EndRendering() override;
			virtual void SetDisplaySize(const SizeF &sz) override;
			virtual const SizeF& GetDisplaySize() const override;
			virtual const PointF& GetDisplayDPI() const override;
			virtual uint32_t GetMaximumTextureSize() const override;

		private:
			/**
			 * Ruft die Gr��e des Viewport ab.
			 *
			 * \return Gr��e des Viewport
			 */
			SizeF GetViewportSize();

			/**
			 * Entfernt ung�ltige Referenzen zu Objekten, f�r die ein Reset ausgef�hrt werden muss.
			 */
			void RemoveWeakReferences();

			IDevice11 device;
			
			SizeF displaySize;
			PointF displayDPI;
			
			RenderTargetPtr defaultTarget;
			
			typedef std::vector<std::weak_ptr<Direct3D11TextureTarget>> TextureTargetList;
			TextureTargetList textureTargets;
			
			typedef std::vector<std::weak_ptr<Direct3D11Texture>> TextureList;
			TextureList textures;
			
			ID3DX11Effect* effect;
			ID3DX11EffectTechnique* normalClippedTechnique;
			ID3DX11EffectTechnique* normalUnclippedTechnique;
			ID3DX11EffectTechnique* premultipliedClippedTechnique;
			ID3DX11EffectTechnique* premultipliedUnclippedTechnique;
			ID3D11InputLayout* inputLayout;
			
			ID3DX11EffectMatrixVariable* worldMatrixVariable;
			ID3DX11EffectMatrixVariable* projectionMatrixVariable;
			ID3DX11EffectShaderResourceVariable* boundTextureVariable;
			ID3DX11EffectScalarVariable* useShaderTextureVariable;
		};
	}
}

#endif