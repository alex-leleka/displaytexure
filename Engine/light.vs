////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer LightVertexBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	float3 lightDirection; // +1
	float3 eyePos; // + 1
	float2 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 h : TEXCOORD1;
	float4 diff : COLOR0;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;
    

	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
	float3 vertexWorldPos = output.position;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);
	float3 V = normalize(eyePos - vertexWorldPos);
	output.h = normalize(-lightDirection + V);

	{	// Minnaert shading
		float LdotN = saturate(dot(-lightDirection, input.normal));
		float VdotN = saturate(dot(V, input.normal));
		float4 matDiffuse = 1;
		float Pow = 0.9;
		float4 ambient = float4(0.4, 0.4, 0.4, 1); // it looks very dark without it
		float4 cDiff = matDiffuse * LdotN * pow(VdotN * LdotN, Pow);
		output.diff = saturate(cDiff + ambient);
	}
    return output;
}

PixelInputType BlinnPhongLightVertexShader(VertexInputType input)
{
	PixelInputType output;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	float3 vertexWorldPos = output.position;
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);
	float3 V = normalize(eyePos - vertexWorldPos);
	output.h = normalize(-lightDirection + V);

	return output;
}