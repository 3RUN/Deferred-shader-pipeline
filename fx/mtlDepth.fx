
const float4x4 matWorldViewProj;
const float4x4 matWorld;

float4 vecViewPos;

texture entSkin1;

sampler TexSampler = sampler_state { Texture = <entSkin1>; Mipfilter = Point; };


void DepthVS (
in float4 inPos	: POSITION,
in float4 inTex	: TEXCOORD0,
out float4 outPos	: POSITION,
out float3 outWorld: TEXCOORD0,
out float2 outTex	: TEXCOORD1 )
{
	outPos = mul ( inPos, matWorldViewProj );
	outWorld = mul ( inPos, matWorld ).xyz;
	outTex = inTex.xy;
}

float4 DepthPS(
in float3 InWorld	: TEXCOORD0,
in float2 inTex	: TEXCOORD1 ) : COLOR0
{
	float4 Tex = tex2D ( TexSampler, inTex );
	clip(Tex.a - 0.5f);
	return float4( -distance( InWorld.xyz, vecViewPos.xyz ), 0.0f, 0.0f, 1.0f );
}

technique Depth
{
	pass p0
	{
		ZEnable = True;
		ZWriteEnable = True;
		AlphaBlendEnable = False;
		AlphaTestEnable = False;
		
		Lighting	= False;
		
		VertexShader = compile vs_3_0 DepthVS();
		PixelShader  = compile ps_3_0 DepthPS();
	}
}	