////////////////////////////////////////////////////////////////////////////////
// Filename: light.vs
////////////////////////////////////////////////////////////////////////////////
#version 400


/////////////////////
// INPUT VARIABLES //
/////////////////////
in vec3 inputPosition;
in vec3 inputNormal;
in vec2 inputTexCoord;
in vec3 inputTangent;
in vec3 inputBinormal;

//////////////////////
// OUTPUT VARIABLES //
//////////////////////
out vec2 texCoord;
out vec3 normal;
out vec3 tang;
out vec3 binorm;

///////////////////////
// UNIFORM VARIABLES //
///////////////////////
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 plane;

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
void main(void)
{
	// Calculate the position of the vertex against the world, view, and projection matrices.
	gl_ClipDistance[0] = dot(modelMatrix * vec4(inputPosition, 1.0f), plane);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inputPosition, 1.0f);

		// Store the texture coordinates for the pixel shader.
	texCoord = inputTexCoord;
	
	// Calculate the normal vector against the model matrix only.
	normal = vec3(modelMatrix * vec4(inputNormal,0));
	vec3 tangente = vec3(modelMatrix * vec4(inputTangent,0));
	vec3 binormal = vec3(modelMatrix * vec4(inputBinormal,0));

	// Normalize the normal vector.
	normal = normalize(normal);
	tang = normalize(tangente);
	binorm = normalize(binormal);	
	
}