#version 400

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTexCoord;
layout (location = 3) in vec3 inputTangent;
layout (location = 4) in vec3 inputBinormal;

out DATA
{
	vec4 position;
	vec3 normal;
	vec2 uv;
	vec3 binormal;
	vec3 tangent;
} vs_out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

uniform vec4 plane;

void main() {
	vec4 pos = modelMatrix * vec4(inputPosition, 1.0);

	gl_ClipDistance[0] = dot(pos, plane);

	vs_out.position = pos;

	gl_Position = projectionMatrix * viewMatrix * pos;

	mat3 model = mat3(modelMatrix);
	vs_out.normal = model * inputNormal;
	vs_out.binormal = model * inputBinormal;
	vs_out.tangent = model * inputTangent;
	vs_out.uv = vec2(inputTexCoord.x, 1 - inputTexCoord.y);
}