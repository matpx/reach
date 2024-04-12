#pragma pack_matrix(row_major)

cbuffer CB : register(b0)
{
    float4x4 g_Transform;
};

struct VsOutput {
	float3 normal : NORMAL;
	float4 pos : SV_Position;
};

VsOutput main_vs(
	float3 i_pos : POSITION,
	float3 i_normal : NORMAL
)
{
    VsOutput v1;

    v1.normal = i_normal;
    v1.pos = mul(float4(i_pos, 1), g_Transform);

    return v1;
}

void main_ps(
	VsOutput v1,
	out float4 o_color : SV_Target0
)
{
    o_color = float4(v1.normal, 1);
}
