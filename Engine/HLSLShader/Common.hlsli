
// Lambert cosine law.
float CalcLambert(float3 worldNormal, float3 lightDirection)
{
    return saturate(dot(worldNormal, -lightDirection));
}

// Half Lambert.
float CalcHalfLambert(
    float3 worldNormal, 
    float3 lightDirection,
    float param = 2)
{
    float nDotL = CalcLambert(worldNormal, lightDirection);
    return pow(nDotL * 0.5f + 0.5f, param);
}