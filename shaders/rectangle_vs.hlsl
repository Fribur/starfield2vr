struct VertexToPixel
{
	float4 position:  SV_POSITION;
	float2 tex_coord:		  TEXCOORD;
};

VertexToPixel vs_main(uint id: SV_VERTEXID)
{
	VertexToPixel output;

	//getting the uv coordinates of the triangle
	output.tex_coord = float2((id << 1) & 2, id & 2);

	//converting them to screen space (-1,-1) to (-3,-3)
	output.position = float4(output.tex_coord, 0.0, 1.0);
	output.position.x = output.position.x * 2 - 1;
	output.position.y = output.position.y * (-2) + 1;

	return output;

}