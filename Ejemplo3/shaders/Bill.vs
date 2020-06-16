#version 400

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTexCoord;
layout (location = 3) in vec3 inputTangent;
layout (location = 4) in vec3 inputBinormal;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);

uniform vec4 plane;

void main() {
	vec4 pos = modelMatrix * vec4(inputPosition, 1.0);

	gl_ClipDistance[0] = dot(pos, plane);

	gl_Position = projectionMatrix * viewMatrix * pos;

	texCoord = vec2(inputPosition.x/2.0 + 0.5, inputPosition.z/2.0 + 0.5);
}