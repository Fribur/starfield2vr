#include "taa_common.hlsli"

Texture2D<float4> resource_tex : register(t0);
Texture2D<float4> resource_tex_comp : register(t1);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);

float3 R11G11B10ToFloat3(uint packedInput)
{
    float3 result;
    result.r = f16tof32(packedInput & 0x7FF);
    result.g = f16tof32((packedInput >> 11) & 0x7FF);
    result.b = f16tof32((packedInput >> 22) & 0x3FF);
    return result;
}

// Helper function to normalize depth
float NormalizeDepth(float depth, float near, float far)
{
    return (depth - near) / (far - near);
}

float4 VisualizeResource(in Texture2D<float4> resourceTexture, in float2 uv, in uint format)
{
    float4 output = float4(0, 0, 0, 1);
    int3 position = int3(0, 0, 0);
    position.x = int(uv.x * w_width);
    position.y = int(uv.y * w_height);
    switch(format)
    {
        case 48: // DXGI_FORMAT_R8G8_TYPELESS
        {
            float2 value = resourceTexture.Sample(LinearSampler, uv).rg * 10;
            // Assuming it's used for motion vectors, we'll visualize it in RG channels
            output.r = value.r;
            output.g = value.g;
            output.b = 0;
            return output;
        }
        case 26: // DXGI_FORMAT_R11G11B10_FLOAT
        {
            return resourceTexture.Sample(LinearSampler, uv);
        }

        case 33: // DXGI_FORMAT_R16G16_TYPELESS
        {
            float2 value = resourceTexture.Sample(LinearSampler, uv).rg * 10;
            // Visualize as RG channels
            output.r = value.r;
            output.g = value.g;
            output.b = 0;
            break;
        }

        case 53: // DXGI_FORMAT_R16_TYPELESS
        {
            float value = resourceTexture.Sample(LinearSampler, uv).r;
            // Assuming it might be depth, normalize it
            output.rgb = value;
            break;
        }
        case 60: // DXGI_FORMAT_R16_FLOAT
        {
            float value = resourceTexture.Sample(LinearSampler, uv).r;
            output.rgb = value;
            break;
        }

        case 19: // DXGI_FORMAT_R32G8X24_TYPELESS
        {
            float depth = resourceTexture.Sample(LinearSampler, uv).r * 10;
            output.rgb = depth;
            break;
        }

        default:
            output = resourceTexture.Sample(LinearSampler, uv);
    }

    return output;
}


float4 ps_main(VertexOut inp) : SV_TARGET {
    float2 modifiedUV = inp.texcoord;
    modifiedUV.x = modifiedUV.x * 0.5;
    float4 srgbColor =  VisualizeResource(resource_tex, modifiedUV, format11);
    float4 srgbColor2 =  VisualizeResource(resource_tex_comp, modifiedUV, format12);
    float4 resultColor = srgbColor - srgbColor2;
    resultColor = saturate(resultColor); // Clamps the result to [0, 1]
    return modifiedUV.y < 0.5 ? srgbColor : resultColor;
}