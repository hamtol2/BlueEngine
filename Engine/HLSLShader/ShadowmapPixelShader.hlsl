// VS -> PS.
struct VertexToPixel
{
    float4 position : SV_POSITION;
    float4 clipPosition : TEXCOORD0;
};

void main(VertexToPixel input)
{
}