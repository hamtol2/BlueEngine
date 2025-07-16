// VS -> PS.
struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 clipPosition : TEXCOORD1;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    // NDC 공간에서 깊이 값을 [0,1] 범위로 정규화
    float depth = input.clipPosition.z / input.clipPosition.w;
    return float4(depth, depth, depth, 1.0f);
}