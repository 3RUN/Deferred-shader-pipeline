
// comment this to disable certain effects
#define VERTEX_SNAPPING
#define AFFINE_TEX_MAPPING
#define POLY_CUTOFF

float4 vecTime;
float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecEmissive;
float4 vecSkill41;
float4 vecSkill1;
float4 vecSkill5;

float4 vecViewPos;
float4 vecFog;
float4 vecFogColor;

float4x4 matWorldViewProj;
float4x4 matWorld;

texture entSkin1;
sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Linear; Minfilter = Linear; Magfilter = Linear; };

void wavedVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float2 outTex: TEXCOORD1 )
{
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	
	outPos = mul( inPos, matWorldViewProj );
	
	float overallforce = 1.0f - saturate( (outPos.z - vecSkill5.x) / vecSkill5.y );
	
	float force_x = vecSkill41.x * overallforce; 
	float force_y = vecSkill41.y * overallforce;
	float speed = sin((vecTime.w + (outWorld.x + outWorld.z)) * vecSkill41.z);
	
	if( inPos.y > 0 )
	{
		inPos.x += speed * force_x * inPos.y;
		inPos.z += speed * force_y * inPos.y;
		inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
	}
	
	// vertex snapping
	#ifdef VERTEX_SNAPPING
		float4 snapToPixel = mul(inPos, matWorldViewProj);
		float4 vertex = snapToPixel;
		vertex.xyz = snapToPixel.xyz / snapToPixel.w;
		vertex.x = floor((vecSkill1.x + 40) * vertex.x) / (vecSkill1.x + 40); // default 160
		vertex.y = floor(vecSkill1.x * vertex.y) / vecSkill1.x; // default 120
		vertex.xyz *= snapToPixel.w;
		outPos = vertex;
		#else
		outPos = mul(inPos, matWorldViewProj);
	#endif
	
	// affine texture mapping
	#ifdef AFFINE_TEX_MAPPING
		outPos *= inPos.w / length(mul(inPos, matWorldViewProj));
	#endif
	
	outWorld = mul( inPos, matWorld );
	
	// cut out polygons
	#ifdef POLY_CUTOFF
		float distance = length(mul(inPos, matWorldViewProj));
		if(distance > vecSkill1.y)
		{
			outPos.w = 0;
		}
	#endif
}

void stopVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float2 outTex: TEXCOORD1 )
{
	outWorld = mul( inPos, matWorld );
	outTex.xy = inTex1.xy;
	
	float4 view = mul ( inPos, matWorldViewProj );
	float overallforce = 1.0f - saturate( (view.z - vecSkill5.x) / vecSkill5.y );
	
	float force_x = vecSkill41.x * overallforce; 
	float force_y = vecSkill41.y * overallforce;
	float speed = sin((vecTime.w + (outWorld.x + outWorld.z)) * vecSkill41.z);
	
	if( inPos.y > 0 )
	{
		inPos.x += speed * force_x * inPos.y;
		inPos.z += speed * force_y * inPos.y;
		inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
	}
	outPos = mul( inPos, matWorldViewProj );
	outWorld = mul( inPos, matWorld );
}

void flatVS (
in float4 inPos: POSITION,
in float2 inTex1: TEXCOORD0,
out float4 outPos: POSITION,
out float4 outWorld: TEXCOORD0,
out float2 outTex: TEXCOORD1 )
{
	outTex.xy = inTex1.xy;
	outPos = mul( inPos, matWorldViewProj );
	outWorld = mul( inPos, matWorld );
}

float4 wavedPS (
in float4 inWorld: TEXCOORD0,
in float2 inTex: TEXCOORD1,
out float4 Color1 : COLOR1,
out float4 Color2 : COLOR2,
out float4 Color3 : COLOR3 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, inTex.xy );
	float4 Shadow = (vecAmbient * vecLight) + float4(vecEmissive.xyz * vecColor.xyz, vecLight.w);
	float4 fAlpha = min(vecLight.w, Tex.a);
	
	float fDepth = distance(vecViewPos.xyz, inWorld.xyz);
	float Fog = saturate((fDepth - vecFog.x) * vecFog.z);
	fAlpha = min ( fAlpha, 1.0f - saturate( (fDepth - vecSkill5.z) / vecSkill5.w ) );
	
	float4 Color0;
	Color0.rgb = 0;//Tex.rgb;
	Color0.rgb = lerp ( Color0.rgb, vecFogColor, Fog );
	Color0.a = fAlpha;// > 0.5f ? 1.0f : 0;
	
	Color1.rgb = Tex.rgb;
	Color1.a = fAlpha;
	
	Color2.rgb = inWorld.xyz;
	Color2.a = Color0.a;
	
	Color3.rgb = Shadow.rgb;
	Color3.a = Color0.a;
	
	return Color0;
}

float4 fadePS (
in float4 inWorld: TEXCOORD0,
in float2 inTex: TEXCOORD1,
out float4 Color1 : COLOR1,
out float4 Color2 : COLOR2,
out float4 Color3 : COLOR3 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, inTex.xy );
	float4 Shadow = (vecAmbient * vecLight) + float4(vecEmissive.xyz * vecColor.xyz, vecLight.w);
	float4 fAlpha = min(vecLight.w, Tex.a);
	
	float fDepth = distance ( inWorld.xyz, vecViewPos.xyz );
	float Fog = saturate ( ( fDepth - vecFog.x ) * vecFog.z );
	
	float4 Color0;
	Color0.rgb = 1;//Tex.rgb;
	Color0.rgb = lerp ( Color0.rgb, vecFogColor, Fog );
	Color0.a = sqrt(fAlpha);// < 1 ? fAlpha : 0;
	
	Color1.rgb =  1.0f - ( (fDepth - vecSkill5.x) / vecSkill5.y );//Tex.rgb;
	Color1.a = Color0.a;
	
	Color2.rgb = inWorld.xyz;
	Color2.a = Color1.a;
	
	Color3.rgb = Shadow.rgb;
	Color3.a = Color2.a;
	
	return Color0;
}

technique
{
	pass p0
	{
		ZWriteEnable = True;
		AlphaBlendEnable = True;
		AlphaTestEnable = True;
		
		CullMode = None;
		
		VertexShader = compile vs_3_0 wavedVS();
		PixelShader  = compile ps_3_0 wavedPS();
	}

}

technique fallback { pass p0{} }