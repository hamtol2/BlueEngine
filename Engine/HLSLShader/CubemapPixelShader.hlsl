struct PixelInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

TextureCube skyboxTexture : register(t0);
SamplerState samplerState : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    return skyboxTexture.Sample(samplerState, input.texCoord);
}