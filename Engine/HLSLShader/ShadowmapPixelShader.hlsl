// VS -> PS.
struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 clipPosition : TEXCOORD1;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    float depth = input.clipPosition.z / input.clipPosition.w;
    return float4(depth.xxx, 1);
}