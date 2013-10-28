#include "Direct3D10Renderer.hpp"
#include "Direct3D10Texture.hpp"
#include "Direct3D10GeometryBuffer.hpp"
#include "Direct3D10RenderTarget.hpp"
#include "Direct3D10ViewportTarget.hpp"
#include "Direct3D10TextureTarget.hpp"
#include "../../Misc/Exceptions.hpp"

#include <algorithm>

const char shaderSource[] =
	"matrix WorldMatrix;\n"
	"matrix ProjectionMatrix;\n"
	"Texture2D BoundTexture;\n"
	"bool UseShaderTexture;\n"
	"\n"
	"struct VSSceneIn\n"
	"{\n"
	"	float3 pos : POSITION;\n"
	"	float4 colour : COLOR;\n"
	"	float2 tex : TEXCOORD;\n"
	"};\n"
	"\n"
	"struct PSSceneIn\n"
	"{\n"
	"	float4 pos : SV_Position;\n"
	"	float4 colour : COLOR;\n"
	"	float2 tex : TEXCOORD;\n"
	"};\n"
	"DepthStencilState DisableDepth\n"
	"{\n"
	"	DepthEnable = FALSE;\n"
	"	DepthWriteMask = ZERO;\n"
	"};\n"
	"BlendState BM_NORMAL\n"
	"{\n"
	"	AlphaToCoverageEnable = false;\n"
	"	BlendEnable[0] = true;\n"
	"	SrcBlendAlpha = INV_DEST_ALPHA;\n"
	"	DestBlendAlpha = ONE;\n"
	"	SrcBlend = SRC_ALPHA;\n"
	"	DestBlend = INV_SRC_ALPHA;\n"
	"};\n"
	"BlendState BM_RTT_PREMULTIPLIED\n"
	"{\n"
	"	AlphaToCoverageEnable = false;\n"
	"	BlendEnable[0] = true;\n"
	"	SrcBlendAlpha = ONE;\n"
	"	DestBlendAlpha = INV_SRC_ALPHA;\n"
	"	SrcBlend = ONE;\n"
	"	DestBlend = INV_SRC_ALPHA;\n"
	"};\n"
	"SamplerState LinearSampler\n"
	"{\n"
	"	Filter = MIN_MAG_MIP_LINEAR;\n"
	"	AddressU = Clamp;\n"
	"	AddressV = Clamp;\n"
	"};\n"
	"RasterizerState clipRasterstate\n"
	"{\n"
	"	DepthClipEnable = false;\n"
	"	FillMode = Solid;\n"
	"	CullMode = None;\n"
	"	ScissorEnable = true;\n"
	"};\n"
	"RasterizerState noclipRasterstate\n"
	"{\n"
	"	DepthClipEnable = false;\n"
	"	FillMode = Solid;\n"
	"	CullMode = None;\n"
	"	ScissorEnable = false;\n"
	"};\n"
	"// Vertex shader\n"
	"PSSceneIn VSMain(VSSceneIn input)\n"
	"{\n"
	"	PSSceneIn output = (PSSceneIn)0.0;\n"
	"	output.pos = mul( float4(input.pos, 1), WorldMatrix );\n"
	"	output.pos = mul( output.pos, ProjectionMatrix );\n"
	"	output.tex = input.tex;\n"
	"	output.colour.rgba = input.colour.bgra;\n"
	"	return output;\n"
	"}\n"
	"// Pixel shader\n"
	"float4 PSMain(PSSceneIn input) : SV_Target\n"
	"{\n"
	"	float4 colour;\n"
	"	if (UseShaderTexture)\n"
	"		colour = BoundTexture.Sample(LinearSampler, input.tex) * input.colour;\n"
	"	else\n"
	"		colour = input.colour;\n"
	"	return colour;\n"
	"}\n"
	"// Techniques\n"
	"technique10 BM_NORMAL_Clipped_Rendering\n"
	"{\n"
	"	pass P0\n"
	"	{\n"
	"		SetVertexShader(CompileShader(vs_4_0, VSMain()));\n"
	"		SetGeometryShader(NULL);\n"
	"		SetPixelShader(CompileShader(ps_4_0, PSMain()));\n"
	"		SetDepthStencilState(DisableDepth, 0);\n"
	"		SetBlendState(BM_NORMAL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);\n"
	"		SetRasterizerState(clipRasterstate);\n"
	"	}\n"
	"}\n"
	"technique10 BM_NORMAL_Unclipped_Rendering\n"
	"{\n"
	"	pass P0\n"
	"	{\n"
	"		SetVertexShader(CompileShader(vs_4_0, VSMain()));\n"
	"		SetGeometryShader(NULL);\n"
	"		SetPixelShader(CompileShader(ps_4_0, PSMain()));\n"
	"		SetDepthStencilState(DisableDepth, 0);\n"
	"		SetBlendState(BM_NORMAL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);\n"
	"		SetRasterizerState(noclipRasterstate);\n"
	"	}\n"
	"}\n"
	"technique10 BM_RTT_PREMULTIPLIED_Clipped_Rendering\n"
	"{\n"
	"	pass P0\n"
	"	{\n"
	"		SetVertexShader(CompileShader(vs_4_0, VSMain()));\n"
	"		SetGeometryShader(NULL);\n"
	"		SetPixelShader(CompileShader(ps_4_0, PSMain()));\n"
	"		SetDepthStencilState(DisableDepth, 0);\n"
	"		SetBlendState(BM_RTT_PREMULTIPLIED, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);\n"
	"		SetRasterizerState(clipRasterstate);\n"
	"	}\n"
	"}\n"
	"technique10 BM_RTT_PREMULTIPLIED_Unclipped_Rendering\n"
	"{\n"
	"	pass P0\n"
	"	{\n"
	"		SetVertexShader(CompileShader(vs_4_0, VSMain()));\n"
	"		SetGeometryShader(NULL);\n"
	"		SetPixelShader(CompileShader(ps_4_0, PSMain()));\n"
	"		SetDepthStencilState(DisableDepth, 0);\n"
	"		SetBlendState(BM_RTT_PREMULTIPLIED, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);\n"
	"		SetRasterizerState(noclipRasterstate);\n"
	"	}\n"
	"}\n";

namespace OSHGui
{
	namespace Drawing
	{
		D3DMATRIX Identity =
		{
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};

		Direct3D10Renderer::Direct3D10Renderer(ID3D10Device *_device)
			: device(_device),
			  displaySize(GetViewportSize()),
			  displayDPI(96, 96),
			  normalClippedTechnique(0),
			  normalUnclippedTechnique(0),
			  premultipliedClippedTechnique(0),
			  premultipliedUnclippedTechnique(0),
			  inputLayout(0),
			  boundTextureVariable(0),
			  worldMatrixVariable(0),
			  projectionMatrixVariable(0),
			  defaultTarget(std::make_shared<Direct3D10ViewportTarget>(*this))
		{
			ID3D10Blob *errors = nullptr;
			if (FAILED(D3DX10CreateEffectFromMemory(shaderSource, sizeof(shaderSource), 0, 0, 0, "fx_4_0", 0, 0, device, 0, 0, &effect, &errors, 0)))
			{
				std::string msg(static_cast<const char*>(errors->GetBufferPointer()), errors->GetBufferSize());
				errors->Release();
				
				throw Misc::Exception(msg);
			}

			normalClippedTechnique = effect->GetTechniqueByName("BM_NORMAL_Clipped_Rendering");
			normalUnclippedTechnique = effect->GetTechniqueByName("BM_NORMAL_Unclipped_Rendering");
			premultipliedClippedTechnique = effect->GetTechniqueByName("BM_RTT_PREMULTIPLIED_Clipped_Rendering");
			premultipliedUnclippedTechnique = effect->GetTechniqueByName("BM_RTT_PREMULTIPLIED_Unclipped_Rendering");

			worldMatrixVariable = effect->GetVariableByName("WorldMatrix")->AsMatrix();
			projectionMatrixVariable = effect->GetVariableByName("ProjectionMatrix")->AsMatrix();
			boundTextureVariable = effect->GetVariableByName("BoundTexture")->AsShaderResource();
			useShaderTextureVariable = effect->GetVariableByName("UseShaderTexture")->AsScalar();

			const D3D10_INPUT_ELEMENT_DESC vertexLayout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	  0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0 },
			};

			D3D10_PASS_DESC passDescription;
			if (FAILED(normalClippedTechnique->GetPassByIndex(0)->GetDesc(&passDescription)))
			{
				throw Misc::Exception();
			}

			auto count = sizeof(vertexLayout) / sizeof(vertexLayout[0]);
			if (FAILED(device->CreateInputLayout(vertexLayout, count, passDescription.pIAInputSignature, passDescription.IAInputSignatureSize, &inputLayout)))
			{
				throw Misc::Exception();
			}
		}
		//---------------------------------------------------------------------------
		Direct3D10Renderer::~Direct3D10Renderer()
		{
			if (effect)
			{
				effect->Release();
			}

			if (inputLayout)
			{
				inputLayout->Release();
			}
		}
		//---------------------------------------------------------------------------
		RenderTargetPtr& Direct3D10Renderer::GetDefaultRenderTarget()
		{
			return defaultTarget;
		}
		//---------------------------------------------------------------------------
		GeometryBufferPtr Direct3D10Renderer::CreateGeometryBuffer()
		{
			return std::make_shared<Direct3D10GeometryBuffer>(*this);
		}
		//---------------------------------------------------------------------------
		TextureTargetPtr Direct3D10Renderer::CreateTextureTarget()
		{
			auto textureTarget = std::make_shared<Direct3D10TextureTarget>(*this);
			textureTargets.emplace_back(textureTarget);
			return textureTarget;
		}
		//---------------------------------------------------------------------------
		TexturePtr Direct3D10Renderer::CreateTexture()
		{
			auto texture = std::shared_ptr<Direct3D10Texture>(new Direct3D10Texture(*this));
			textures.emplace_back(texture);
			return texture;
		}
		//---------------------------------------------------------------------------
		TexturePtr Direct3D10Renderer::CreateTexture(const Misc::AnsiString &filename)
		{
			auto texture = std::shared_ptr<Direct3D10Texture>(new Direct3D10Texture(*this, filename));
			textures.emplace_back(texture);
			return texture;
		}
		//---------------------------------------------------------------------------
		TexturePtr Direct3D10Renderer::CreateTexture(const SizeF &size)
		{
			auto texture = std::shared_ptr<Direct3D10Texture>(new Direct3D10Texture(*this, size));
			textures.emplace_back(texture);
			return texture;
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::BeginRendering()
		{
			device->IASetInputLayout(inputLayout);
			device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::EndRendering()
		{

		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::SetDisplaySize(const SizeF &size)
		{
			if (size != displaySize)
			{
				displaySize = size;

				auto area = defaultTarget->GetArea();
				area.SetSize(size);
				defaultTarget->SetArea(area);
			}
		}
		//---------------------------------------------------------------------------
		const SizeF& Direct3D10Renderer::GetDisplaySize() const
		{
			return displaySize;
		}
		//---------------------------------------------------------------------------
		const PointF& Direct3D10Renderer::GetDisplayDPI() const
		{
			return displayDPI;
		}
		//---------------------------------------------------------------------------
		uint32_t Direct3D10Renderer::GetMaximumTextureSize() const
		{
			return 8192;
		}
		//---------------------------------------------------------------------------
		SizeF Direct3D10Renderer::GetViewportSize()
		{
			D3D10_VIEWPORT vp;
			uint32_t count = 1;

			device->RSGetViewports(&count, &vp);

			if (count != 1)
			{
				throw Misc::Exception();
			}
			
			return SizeF(vp.Width, vp.Height);
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::PreD3DReset()
		{
			RemoveWeakReferences();
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::PostD3DReset()
		{
			RemoveWeakReferences();
		}
		//---------------------------------------------------------------------------
		ID3D10Device* Direct3D10Renderer::GetDevice() const
		{
			return device;
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::BindTechniquePass(const BlendMode mode, const bool clipped)
		{
			if (mode == BlendMode::RTT_PreMultiplied)
			{
				if (clipped)
				{
					premultipliedClippedTechnique->GetPassByIndex(0)->Apply(0);
				}
				else
				{
					premultipliedUnclippedTechnique->GetPassByIndex(0)->Apply(0);
				}
			}
			else if (clipped)
			{
				normalClippedTechnique->GetPassByIndex(0)->Apply(0);
			}
			else
			{
				normalUnclippedTechnique->GetPassByIndex(0)->Apply(0);
			}
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::SetCurrentTextureShaderResource(ID3D10ShaderResourceView *srv)
		{
			boundTextureVariable->SetResource(srv);
			useShaderTextureVariable->SetBool(srv != nullptr);
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::SetProjectionMatrix(D3DXMATRIX &matrix)
		{
			projectionMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::SetWorldMatrix(D3DXMATRIX& matrix)
		{
			worldMatrixVariable->SetMatrix(reinterpret_cast<float*>(&matrix));
		}
		//---------------------------------------------------------------------------
		void Direct3D10Renderer::RemoveWeakReferences()
		{
			textureTargets.erase(std::remove_if(std::begin(textureTargets), std::end(textureTargets), [](const std::weak_ptr<Direct3D10TextureTarget> ptr) { return ptr.expired(); }), std::end(textureTargets));
			textures.erase(std::remove_if(std::begin(textures), std::end(textures), [](const std::weak_ptr<Direct3D10Texture> ptr) { return ptr.expired(); }), std::end(textures));
		}
		//---------------------------------------------------------------------------
	}
}