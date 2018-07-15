struct forward_lit
{
	float4 m_albedo;
	float m_roughness;
	float m_reflectivity;
	float3 m_padding;
};

struct forward_lit_instanced
{
	float4 m_albedo;
	float m_roughness;
	float m_reflectivity;
	float3 m_padding;
};

struct forward_lit_skinned
{
	float4 m_albedo;
	float m_roughness;
	float m_reflectivity;
	float3 m_padding;
};

struct gbuffer
{
	float4 m_albedo;
	float m_roughness;
	float m_reflectivity;
	float3 m_padding;
};

struct gbuffer_skinned
{
	float4 m_albedo;
	float m_roughness;
	float m_reflectivity;
	float3 m_padding;
};
