#define SMAA_INCLUDE_VS 1
#define SMAA_INCLUDE_PS 0

#include "smaa_common.hlsli"
#include "SMAA.inc.hlsl"

ResolveVertex vs_main(uint id: SV_VERTEXID)
{
	ResolveVertex output;

	vsScreenQuad(id, output.position, output.texcoord);

	float4 offset;
	SMAANeighborhoodBlendingVS( output.texcoord, offset );

	output.texcoord1 = offset;
	return output;
}