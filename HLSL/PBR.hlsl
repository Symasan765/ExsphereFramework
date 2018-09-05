
#define PI 3.14159265359
#define PI2 6.28318530718
#define RECIPROCAL_PI 0.31830988618
#define RECIPROCAL_PI2 0.15915494
#define LOG2 1.442695
#define EPSILON 1e-6


struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangent : TANGENT;
	int4 bindex : BONEINDEX;
	float4 bweight : WEIGHT;
	float4x4 WorldMat  : MATRIX;    // ワールド変換行列
  uint InstID : SV_InstanceID; // インスタンスID
};

struct VSOut
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float3 worldPos : POSITION;
};

struct GeometricContext {
  float3 position;
  float3 normal;
  float3 viewDir;
};

// 入射光のパラメータ
struct IncidentLight {
  float3 color;
  float3 direction;
  bool visible;
};

// 反射光のパラメータ
struct ReflectedLight {
  float3 directDiffuse;
  float3 directSpecular;
  float3 indirectDiffuse;
  float3 indirectSpecular;
};

struct DirectionalLight {
  float3 direction;
  float3 color;
};

struct PointLight {
  float3 position;
  float3 color;
  float distance;
  float decay;
};

struct SpotLight {
  float3 position;
  float3 direction;
  float3 color;
  float distance;
  float decay;
  float coneCos;
  float penumbraCos;
};

struct Material {
  float3 diffuseColor;
  float specularRoughness;
  float3 specularColor;
};

// light uniforms
#define MAX_POINT_LIGHT 48

Texture2D g_AlbedoTex : register(t0);
SamplerState g_samLinear : register(s0);

cbuffer Scene  : register(b0)
{
	float4x4 worldViewProjMatrix;
	float4x4 worldViewMatrix;
};

cbuffer LightData  : register(b1)
{
	float4 DirectionalLightDirection;
	float4 DirectionalLightColor;
};


cbuffer PointLightData  : register(b2)
{
	float4 PointLightPosition[MAX_POINT_LIGHT];
	float4 PointLightColor[MAX_POINT_LIGHT];
	float PointLightDistance[MAX_POINT_LIGHT];
	float PointLightDecay[MAX_POINT_LIGHT];
};


cbuffer PbrParameter  : register(b3)
{
	float metallic;
    float roughness;
};

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

VSOut VSMain(VSIn vsIn)
{
	VSOut output;
	
	float4 pos = 1.0f;
	float4 normal = 0.0f;
	pos.xyz = vsIn.pos;
	normal.xyz = vsIn.normal;

	output.pos = mul(pos, (vsIn.WorldMat));	
	output.pos = mul(output.pos, (worldViewProjMatrix));
	
	output.normal = mul(vsIn.normal.xyz, ((float3x3)transpose(inverse(worldViewMatrix * vsIn.WorldMat))));
	output.uv = vsIn.uv;
	output.worldPos = mul(pos, worldViewMatrix);
	output.worldPos = -output.worldPos;
	return output;
}


// lights

bool testLightInRange(const in float lightDistance, const in float cutoffDistance) {
  return any(bool2(cutoffDistance == 0.0, lightDistance < cutoffDistance));
}

float punctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent) {
  if (decayExponent > 0.0) {
    return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
  }
  return 1.0;
}

void getDirectionalDirectLightIrradiance(const in DirectionalLight directionalLight, const in GeometricContext geometry, out IncidentLight directLight) {
  directLight.color = directionalLight.color;
  directLight.direction = directionalLight.direction;
  directLight.visible = true;
}

void getPointDirectLightIrradiance(const in PointLight pointLight, const in GeometricContext geometry, out IncidentLight directLight) {
  float3 L = pointLight.position - geometry.position;
  directLight.direction = normalize(L);
  
  float lightDistance = length(L);
  if (testLightInRange(lightDistance, pointLight.distance)) {
    directLight.color = pointLight.color;
    directLight.color *= punctualLightIntensityToIrradianceFactor(lightDistance, pointLight.distance, pointLight.decay);
    directLight.visible = true;
  } else {
    directLight.color = (float3)(0.0);
    directLight.visible = false;
  }
}

void getSpotDirectLightIrradiance(const in SpotLight spotLight, const in GeometricContext geometry, out IncidentLight directLight) {
  float3 L = spotLight.position - geometry.position;
  directLight.direction = normalize(L);
  
  float lightDistance = length(L);
  float angleCos = dot(directLight.direction, spotLight.direction);
  
  if (all(bool2(angleCos > spotLight.coneCos, testLightInRange(lightDistance, spotLight.distance)))) {
    float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
    directLight.color = spotLight.color;
    directLight.color *= spotEffect * punctualLightIntensityToIrradianceFactor(lightDistance, spotLight.distance, spotLight.decay);
    directLight.visible = true;
  } else {
    directLight.color = (float3)(0.0);
    directLight.visible = false;
  }
}

// BRDF
// Normalized Lambert
float3 DiffuseBRDF(float3 diffuseColor) {
  return diffuseColor / PI;
}

float3 F_Schlick(float3 specularColor, float3 H, float3 V) {
  return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V,H)), 5.0));
}

float D_GGX(float a, float dotNH) {
  float a2 = a*a;
  float dotNH2 = dotNH*dotNH;
  float d = dotNH2 * (a2 - 1.0) + 1.0;
  return a2 / (PI * d * d);
}

float G_Smith_Schlick_GGX(float a, float dotNV, float dotNL) {
  float k = a*a*0.5 + EPSILON;
  float gl = dotNL / (dotNL * (1.0 - k) + k);
  float gv = dotNV / (dotNV * (1.0 - k) + k);
  return gl*gv;
}

// Cook-Torrance
float3 SpecularBRDF(const in IncidentLight directLight, const in GeometricContext geometry, float3 specularColor, float roughnessFactor) {
  
  float3 N = geometry.normal;
  float3 V = geometry.viewDir;
  float3 L = directLight.direction;
  
  float dotNL = saturate(dot(N,L));
  float dotNV = saturate(dot(N,V));
  float3 H = normalize(L+V);
  float dotNH = saturate(dot(N,H));
  float dotVH = saturate(dot(V,H));
  float dotLV = saturate(dot(L,V));
  float a = roughnessFactor * roughnessFactor;

  float D = D_GGX(a, dotNH);
  float G = G_Smith_Schlick_GGX(a, dotNV, dotNL);
  float3 F = F_Schlick(specularColor, V, H);
  return (F*(G*D))/(4.0*dotNL*dotNV+EPSILON);
}

// RenderEquations(RE)
void RE_Direct(const in IncidentLight directLight, const in GeometricContext geometry, const in Material material, inout ReflectedLight reflectedLight) {
  
  float dotNL = saturate(dot(geometry.normal, directLight.direction));
  float3 irradiance = dotNL * directLight.color;
  
  // punctual light
  irradiance *= PI;
  
  reflectedLight.directDiffuse += irradiance * DiffuseBRDF(material.diffuseColor);
  reflectedLight.directSpecular += irradiance * SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}

float4 PSMain(VSOut vsOut) : SV_TARGET
{
	float3 vViewPosition = vsOut.worldPos;
	float3 vNormal = vsOut.normal;
	float4 albedoMap = g_AlbedoTex.Sample(g_samLinear, (float2)(vsOut.uv));
	float3 albedo = albedoMap.rgb;
	
  GeometricContext geometry;
  geometry.position = -vViewPosition;
  geometry.normal = normalize(vNormal);
  geometry.viewDir = normalize(vViewPosition);
  
  Material material;
  material.diffuseColor = lerp(albedo, (float3)(0.0,0.0,0.0), metallic);
  material.specularColor = lerp((float3)(0.04), albedo, metallic);
  material.specularRoughness = roughness;


	// Lighting
  
  ReflectedLight reflectedLight = (ReflectedLight)(0);
  float3 emissive = (float3)(0.0);
  float opacity = 1.0;
  
  IncidentLight directLight;

	// point light
  for (int i=0; i<MAX_POINT_LIGHT; ++i) {
		PointLight pointLights = (PointLight)(0);
		pointLights.color = PointLightColor[i].xyz;
		pointLights.distance = PointLightDistance[i];
		pointLights.decay = PointLightDecay[i];
		pointLights.position  = PointLightPosition[i].xyz;
    getPointDirectLightIrradiance(pointLights, geometry, directLight);
    if (directLight.visible) {
      RE_Direct(directLight, geometry, material, reflectedLight);
    }
  }

	// directional light
	DirectionalLight directionalLights;
	directionalLights.color = DirectionalLightColor.xyz;
	directionalLights.direction = DirectionalLightDirection.xyz;
    getDirectionalDirectLightIrradiance(directionalLights, geometry, directLight);
    RE_Direct(directLight, geometry, material, reflectedLight);
  

	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
  
	albedoMap.xyz = outgoingLight;
	return albedoMap;
}