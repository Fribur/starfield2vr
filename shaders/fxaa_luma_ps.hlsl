struct VertexToPixel
{
	float4 position:  SV_POSITION;
	float2 tex_coord: TEXCOORD;
};

Texture2D sceneTexture : register(t0);
SamplerState sampler0 : register(s0);

float4 GammaToLinear(float4 color)
{
    return float4(pow(color.rgb, 2.35), color.a);
}

float4 ps_main(VertexToPixel inp) : SV_TARGET {
    float4 color = sceneTexture.Sample(sampler0, inp.tex_coord);
//     color.a = dot(color.rgb, float3(0.299, 0.587, 0.114));
//     color.a = dot(color.rgb, float3(0.2126 , 0.7152, 0.0722)); SRGB
//     color.a = sqrt(sqrt(0.299 * color.r * color.r + 0.587 * color.g * color.g + 0.114 * color.b * color.b));
    //color = GammaToLinear(color);
    color.a = sqrt(dot(color.rgb * color.rgb, float3(0.299, 0.587, 0.114)));

    return color;
}
