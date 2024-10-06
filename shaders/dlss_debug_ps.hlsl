#include "dlss_common.hlsli"

Texture2D<float2> mvec_tex : register(t0);
Texture2D<float> depth_tex : register(t1);

SamplerState LinearSampler : register(s0);
SamplerState PointSampler : register(s1);

float4 ps_main(VertexOut inp) : SV_TARGET {
    float2 velocity = mvec_tex.Sample(PointSampler, inp.texcoord);
    float nonLinearDepth = depth_tex.Sample(LinearSampler, inp.texcoord);
    velocity *= mvecScale;
//
//     float2 uvCurrent = inp.texcoord;
//
//     float4 screenSpacePosCurrent = float4(float2(uvCurrent.x * 2.0 - 1.0, 1.0 - uvCurrent.y * 2.0), nonLinearDepth, 1.0);
//     float4 screenSpacePosPrevious = mul(currentToPreviousClipMatrixNoOffset, screenSpacePosCurrent);
//     float2 uvPrevious = float2(0.5, -0.5) * screenSpacePosPrevious.xy / screenSpacePosPrevious.w + 0.5;
//     float2 velocity2 = uvCurrent - uvPrevious;
//     velocity += velocity2;
    velocity = abs(velocity);
    velocity *= 10.0;
    return float4(velocity.x, velocity.y, 0.0f, 0.0f);
//     return float4(nonLinearDepth.x, nonLinearDepth.x, nonLinearDepth.x, 1.0f);
}