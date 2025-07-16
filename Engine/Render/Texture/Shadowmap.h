#pragma once

#include "Texture.h"
#include "Core/Type.h"
#include <d3d11.h>

namespace Blue
{
    class Shadowmap : public Texture
    {
    public:
        Shadowmap(uint32 width = 4096, uint32 height = 4096);
        ~Shadowmap();

        void Clear();
        virtual void Bind(uint32 index = 0) override;
        void BindSamplerState();
        void Unbind();

        ID3D11ShaderResourceView* GetShaderResourceView() const { return textureData->shaderResourceView; }

    private:

        uint32 width = 0;
        uint32 height = 0;

        ID3D11Texture2D* shadowMapTexture = nullptr;
        ID3D11DepthStencilView* depthStencilView = nullptr;
        D3D11_VIEWPORT shadowmapViewport;

        // 쉐도우 맵 셰이더.
        std::unique_ptr<class ShadowmapShader> shadowmapShader;
    };
}