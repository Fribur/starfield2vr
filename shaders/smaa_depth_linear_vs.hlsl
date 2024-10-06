#include "smaa_common.hlsli"

ResolveVertex vs_main(uint id: SV_VERTEXID)
{
	ResolveVertex output;
	vsScreenQuad(id, output.position, output.texcoord);
	return output;

}