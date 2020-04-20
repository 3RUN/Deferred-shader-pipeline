
float4 vecViewPort;

texture TargetMap;

sampler ColorSampler = sampler_state
{ 
	Texture = <TargetMap>; 
	MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
	AddressU = Clamp; AddressV = Clamp;
};

float4 PS ( in float2 inTex: TEXCOORD0 ) : COLOR0
{
	float2 Coord = inTex.xy;	
	float4 Color = tex2D ( ColorSampler, Coord );
	Color.a = 1.0f;
	return Color;
}

technique
{
	pass
	{
		VertexShader = null;
		PixelShader  = compile ps_2_0 PS();
	}
}
