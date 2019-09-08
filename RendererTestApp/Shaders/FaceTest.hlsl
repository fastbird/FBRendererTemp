cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};

struct VertexIn
{
	float3 PosL : POSITION;
};
struct VertexOut
{
	float4 PosH : SV_POSITION;
	float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	// Transform to homogeneous clip space.
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	// Just pass vertex color into the pixel shader.
	vout.Color = float4(1, 1, 1, 1);
	return vout;
}