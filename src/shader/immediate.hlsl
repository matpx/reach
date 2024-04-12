#pragma pack_matrix(row_major)

cbuffer CB : register(b0)
{
    float4x4 g_Transform;
};

struct VsOutput {
	float4 color : COLOR0;
	float4 pos : SV_Position;
};

VsOutput main_vs(
	float3 i_pos : POSITION,
	float4 i_color : COLOR0
)
{
	VsOutput v1;

    v1.color = i_color;
    v1.pos = mul(float4(i_pos, 1), g_Transform);

	return v1;
}

float4 main_ps(
	VsOutput v1
) : SV_Target
{
    return v1.color;
}
