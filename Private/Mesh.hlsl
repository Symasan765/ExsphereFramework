
struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	int4 bindex : BONEINDEX;
	float4 bweight : WEIGHT;
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 Spos : POSITION;
};

//スキニング後の頂点・法線が入る
struct Skin
{
    float4 Pos;
    float4 Normal;
};

cbuffer Scene  : register(b0)
{
	float4x4 worldViewProjMatrix;
	float4x4 worldMatrix;
};

cbuffer Bones  : register(b1){
	matrix g_mConstBoneWorld[128];		// この100はテスト用に128個のボーンを使用したため意味のないマジックナンバー
}

Texture2D g_ColorTex : register(t0);
SamplerState g_samLinear : register(s0);

//ボーン情報を元にしてローカル座標を修正する関数
Skin SkinVert(float4 Pos, float4 Normal, uint4 Bones, float4 Weights)
{
    Skin output = (Skin) 0;
    //ボーン0
    uint iBone = Bones.x;
    float fWeight = Weights.x;
    matrix m = g_mConstBoneWorld[iBone];
    output.Pos += fWeight * mul(Pos, m);
    output.Normal += fWeight * mul(Normal, m);
    
	//ボーン1
    iBone = Bones.y;
    fWeight = Weights.y;
    m = g_mConstBoneWorld[iBone];
    output.Pos += fWeight * mul(Pos, m);
    output.Normal += fWeight * mul(Normal, m);
    
	//ボーン2
    iBone = Bones.z;
    fWeight = Weights.z;
    m = g_mConstBoneWorld[iBone];
    output.Pos += fWeight * mul(Pos, m);
    output.Normal += fWeight * mul(Normal, m);
    
	//ボーン3
    iBone = Bones.w;
    fWeight = Weights.w;
    m = g_mConstBoneWorld[iBone];
    output.Pos += fWeight * mul(Pos, m);
    output.Normal += fWeight * mul(Normal, m);

    return output;
}

VSOut VSMain(VSIn vsIn)
{
	VSOut output;
	
	//ボーンを適用したローカル座標に更新する
	float4 pos = 1.0f;
	float4 normal = 0.0f;
	pos.xyz = vsIn.pos;
	normal.xyz = vsIn.normal;
    Skin skinPos = SkinVert(pos, normal, vsIn.bindex, vsIn.bweight);
    vsIn.pos = skinPos.Pos.xyz;
    vsIn.normal = skinPos.Normal;
	
	output.pos = mul(float4(vsIn.pos.xyz, 1.0f), worldViewProjMatrix);
	
	output.normal = mul(vsIn.normal.xyz, (float3x3)(worldMatrix));
	output.uv = vsIn.uv;
	output.Spos = vsIn.bweight;
	return output;
}

float4 PSMain(VSOut vsOut) : SV_TARGET
{
	float4 ret =0.0f;
	ret = g_ColorTex.Sample(g_samLinear, (float2)(vsOut.uv));
	return ret;
}