// FXAA.hlsl

struct PS_INPUT
{
   float4 position : SV_POSITION;
   float2 texCoord : TEXCOORD0;
};

cbuffer FXAAParams : register(b0)
{
    float2 screenSize;
};

Texture2D inputTexture : register(t0);
SamplerState inputSampler : register(s0);

float4 main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
    float2 inverseScreenSize = 1.0 / screenSize;
    
    float3 rgbM = inputTexture.Sample(inputSampler, texCoord).rgb;
    float3 rgbNW = inputTexture.Sample(inputSampler, texCoord + float2(-1, -1) * inverseScreenSize).rgb;
    float3 rgbNE = inputTexture.Sample(inputSampler, texCoord + float2(1, -1) * inverseScreenSize).rgb;
    float3 rgbSW = inputTexture.Sample(inputSampler, texCoord + float2(-1, 1) * inverseScreenSize).rgb;
    float3 rgbSE = inputTexture.Sample(inputSampler, texCoord + float2(1, 1) * inverseScreenSize).rgb;

    float lumaM = dot(rgbM, float3(0.299, 0.587, 0.114));
    float lumaNW = dot(rgbNW, float3(0.299, 0.587, 0.114));
    float lumaNE = dot(rgbNE, float3(0.299, 0.587, 0.114));
    float lumaSW = dot(rgbSW, float3(0.299, 0.587, 0.114));
    float lumaSE = dot(rgbSE, float3(0.299, 0.587, 0.114));

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float edgeThreshold = 0.125;
    if (lumaMax - lumaMin < edgeThreshold)
    {
        return float4(rgbM, 1.0);
    }

    float lumaSum = lumaNW + lumaNE + lumaSW + lumaSE;
    float2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y = ((lumaNW + lumaSW) - (lumaNE + lumaSE));

    float dirReduce = max((lumaSum * 0.25) * 0.5, 1.0 / 32.0);
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(float2(8, 8), max(float2(-8, -8), dir * rcpDirMin)) * inverseScreenSize;

    float3 rgbA = 0.5 * (
        inputTexture.Sample(inputSampler, texCoord + dir * (1.0 / 3.0 - 0.5)).rgb +
        inputTexture.Sample(inputSampler, texCoord + dir * (2.0 / 3.0 - 0.5)).rgb);
    float3 rgbB = rgbA * 0.5 + 0.25 * (
        inputTexture.Sample(inputSampler, texCoord + dir * 0.5).rgb +
        inputTexture.Sample(inputSampler, texCoord).rgb);

    float lumaB = dot(rgbB, float3(0.299, 0.587, 0.114));
    if (lumaB < lumaMin || lumaB > lumaMax)
    {
        return float4(rgbA, 1.0);
    }
    return float4(rgbB, 1.0);
}
