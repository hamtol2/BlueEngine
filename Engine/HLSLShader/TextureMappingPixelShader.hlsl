#include "Common.hlsli"

struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 cameraDirection : TEXCOORD1;
};

// Texture.
Texture2D diffuseMap : register(t0);
SamplerState diffuseSampler : register(s0);

float4 main(PixelInput input) : SV_TARGET
{
    // Sampling.
    float4 texColor = diffuseMap.Sample(diffuseSampler, input.texCoord);
    
    // Light Dir.
    float3 lightDir = -float3(500.0f, 500.0f, -500.0f);
    lightDir = normalize(lightDir);
    
    // World Normal.
    float3 worldNormal = normalize(input.normal);
    
    // Dot (Lambert cosine law) - diffuse.
    //float nDotl = saturate(dot(worldNormal, -lightDir));
    float nDotl = CalcLambert(worldNormal, lightDir);
    //float nDotl1 = max(0, dot(worldNormal, -lightDir));
    
    // Half Lambert.
    //nDotl = pow((nDotl * 0.7f) + (1.0 - 0.7f), 1);
    
    // Phong (specular).
    
    
    float4 finalColor = texColor * nDotl;

	return finalColor;
}