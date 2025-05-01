
// Ambient - Hemispheric ambient lighting.
float3 CalcAmbient(float3 worldNormal)
{
    // Convert [-1,1] to [0,1]
    float up = worldNormal.y * 0.5f + 0.5f;
    
    // Upper hemisphere ambient color.
    float3 ambientUp = float3(0.2, 0.2, 0.2);
    
    // Down hemisphere ambient color.
    float3 ambientDown = float3(0.1, 0.1, 0.1);
    
    // Calculate ambient value.
    float3 ambient = ambientDown + up * ambientUp;
    return ambient;
}

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

// Phong.
float CalcPhong(
    float3 worldNormal,
    float3 lightDirection,
    float3 cameraDirection,
    float shineness = 16.0f)
{
    float nDotl = CalcLambert(worldNormal, lightDirection);

    float specular = 0;
    if (nDotl)
    {
        float3 reflection = reflect(lightDirection, worldNormal);
        float3 viewDirection = normalize(cameraDirection);
        float rDotv = saturate(dot(reflection, -viewDirection));
        specular = pow(rDotv, shineness);
    }
    
    return specular;
}

float CalcBlinnPhong(
    float3 worldNormal,
    float3 lightDirection,
    float3 cameraDirection,
    float shineness = 32.0f)
{
    float NoL = CalcLambert(worldNormal, lightDirection);

    float specular = 0;
    if (NoL > 0)
    {
        // Half Vector.
        float3 viewDirection = normalize(cameraDirection);
        float3 halfVector = normalize((-lightDirection) + (-viewDirection));
        
        // nDoth.
        float NoH = saturate(dot(worldNormal, halfVector));
        specular = pow(NoH, shineness);
    }
    
    return specular;
}