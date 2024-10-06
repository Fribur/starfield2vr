#define SMAA_INCLUDE_VS 0
#define SMAA_INCLUDE_PS 1

#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"

Texture2D t_CurrentColor	: register(t1);
Texture2D t_SmaaBlend		: register(t3);

float4 ps_main(ResolveVertex inp) : SV_TARGET
{
	float4 color = SMAANeighborhoodBlendingPS(
	               inp.texcoord,
				   inp.texcoord1,
				   t_CurrentColor,
				   t_SmaaBlend);
	return color;
}
