////////////////////////////////////////////////////////////////////////////////
// Filename: light_PS.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
    float3 lightDirection;
    float specularPower;
    float4 specularColor;
    float3 pos;
    float range;
    float3 dir;
    float cone;
    float3 att;
    float padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
    float4 worldPos : POSITION;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float lightIntensity;
	float4 color;
    float3 spotlightColor;
    float4 finalColor;
	float3 reflection;
    float4 specular;


	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.tex);

	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Initialize the specular color.
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

	if(lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);

	    // Saturate the ambient and diffuse color.
		color = saturate(color);

		// Calculate the reflection vector based on the light intensity, normal vector, and light direction.
        reflection = normalize(2 * lightIntensity * input.normal - lightDir); 

		// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower);
    }

    // Add the specular component last to the output color.
    color = saturate(color + specular);

    // Multiply the texture pixel and the input color to get the textured result.
    color = color * textureColor;


    ///////////////
    // Spotlight //
    ///////////////

    input.normal = normalize(input.normal);

    float4 diffuse = shaderTexture.Sample(SampleType, input.tex);

    spotlightColor = float3(0.0f, 0.0f, 0.0f);
	
	//Create the vector between light position and pixels position
    float3 lightToPixelVec = pos - input.worldPos;
		
	//Find the distance between the light pos and pixel pos
    float d = length(lightToPixelVec);
	
	//Add the ambient light
    float3 finalAmbient = diffuse * (diffuseColor * lightIntensity);

	//If pixel is too far, return directional light
    if (d > range)
        return color;
		
	//Turn lightToPixelVec into a unit length vector describing
	//the pixels direction from the lights position
    lightToPixelVec /= d;
		
	//Calculate how much light the pixel gets by the angle
	//in which the light strikes the pixels surface
    float howMuchLight = dot(lightToPixelVec, input.normal);

	//If light is striking the front side of the pixel
    if (howMuchLight > 0.0f)
    {
		//Add light to the finalColor of the pixel
        spotlightColor += diffuse * (diffuseColor * lightIntensity);
					
		//Calculate Light's Distance Falloff factor
        spotlightColor /= (att[0] + (att[1] * d)) + (att[2] * (d * d));

		//Calculate falloff from center to edge of pointlight cone
        spotlightColor *= pow(max(dot(-lightToPixelVec, dir), 0.0f), cone);
    }
	
	//make sure the values are between 1 and 0, and add the ambient
    spotlightColor = saturate(spotlightColor  + finalAmbient*0.2f);

	// Add the specular component last to the output color.
    finalColor = saturate(color + float4(spotlightColor, diffuse.a));

    return finalColor;
}