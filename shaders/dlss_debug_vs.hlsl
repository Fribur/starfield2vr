#include "dlss_common.hlsli"

VertexOut vs_main(uint id: SV_VERTEXID)
{
	VertexOut output;
	vsScreenQuad(id, output.position, output.texcoord);
	return output;
}