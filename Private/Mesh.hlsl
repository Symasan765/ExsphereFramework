
struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 Spos : POSITION;
};

cbuffer Scene
{
	float4x4 worldViewProjMatrix;
	float4x4 worldMatrix;
};

Texture2D g_ColorTex : register(t0);
SamplerState g_samLinear : register(s0);

VSOut VSMain(VSIn vsIn)
{
	VSOut output;
	output.pos = mul(float4(vsIn.pos.xyz, 1.0f), worldViewProjMatrix);
	output.normal = mul(vsIn.normal.xyz, (float3x3)(worldMatrix));
	output.uv = vsIn.uv;
	output.Spos = mul(float4(vsIn.pos.xyz, 1.0f), worldViewProjMatrix);
	return output;
}

float4 PSMain(VSOut vsOut) : SV_TARGET
{
	return g_ColorTex.Sample(g_samLinear, (float2)(vsOut.uv.xy));
	vsOut.Spos.xyz /= vsOut.Spos.w;
	vsOut.Spos.xy+=1.0f;
	vsOut.Spos.xy/=2.0f;
	float4 ret = g_ColorTex.Sample(g_samLinear, (float2)(vsOut.Spos.xy));
	//float4 ret = vsOut.pos / 1000.0f;
	ret.w = 1.0f;
	ret.xyz = vsOut.normal.xyz;
	return ret;
}