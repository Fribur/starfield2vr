struct VertexToPixel
{
	float4 position:  SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

Texture2D highResTexture : register(t0);
SamplerState samplerState : register(s0);

static float gamma = 2.41;

float4 LinearToGamma(float4 color)
{
    return float4(pow(color.rgb, 1.0 / gamma), color.a);
}

float4 GammaToLinear(float4 color)
{
    return float4(pow(color.rgb, gamma), color.a);
}



float4 ps_main(VertexToPixel inp) : SV_TARGET {
//     float w,h;
//     highResTexture.GetDimensions(w,h);
//
//     float2 invHighResSize = float2(1.0/w, 1.0/h);
//     // Downsample in linear space
//     float4 value0 = highResTexture.Sample(samplerState, inp.tex_coord);
//     float4 value1 = highResTexture.Sample(samplerState, inp.tex_coord + float2(invHighResSize.x, 0));
//     float4 value2 = highResTexture.Sample(samplerState, inp.tex_coord + float2(0, invHighResSize.y));
//     float4 value3 = highResTexture.Sample(samplerState, inp.tex_coord + float2(invHighResSize.x, invHighResSize.y));
//     float4 value4 = highResTexture.Sample(samplerState, inp.tex_coord + float2(-invHighResSize.x, 0));
//     float4 value5 = highResTexture.Sample(samplerState, inp.tex_coord + float2(0, -invHighResSize.y));
//     float4 value6 = highResTexture.Sample(samplerState, inp.tex_coord + float2(-invHighResSize.x, -invHighResSize.y));
//     float4 value7 = highResTexture.Sample(samplerState, inp.tex_coord + float2(-invHighResSize.x, invHighResSize.y));
//     float4 value8 = highResTexture.Sample(samplerState, inp.tex_coord + float2(invHighResSize.x, -invHighResSize.y));
//
//
//     float4 gatherValue;
//     gatherValue.rgb = pow(value0.rgb, gamma) + pow(value1.rgb, gamma) + pow(value2.rgb, gamma) + pow(value3.rgb, gamma);
//     gatherValue.rgb += pow(value4.rgb, gamma) + pow(value5.rgb, gamma) + pow(value6.rgb, gamma) + pow(value7.rgb, gamma);
//     gatherValue.rgb += pow(value8.rgb, gamma);
//     gatherValue.a   = value0.a + value1.a + value2.a + value3.a;
//     gatherValue.a  += value4.a + value5.a + value6.a + value7.a;
//     gatherValue.a  += value8.a;
//     return float4(pow(gatherValue.rgb/9, 1.0/gamma), gatherValue.a/9);
    return highResTexture.Sample(samplerState, inp.tex_coord);
}