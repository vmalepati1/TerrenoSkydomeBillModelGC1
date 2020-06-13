#version 400

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTexCoord;
layout (location = 3) in vec3 inputTangent;
layout (location = 4) in vec3 inputBinormal;

out vec2 textureCoords;

uniform mat4 projectionMatrix = mat4(1.0);
uniform mat4 transformation = mat4(1.0);

void main() {
	gl_ClipDistance[0] = 1;
	gl_Position = projectionMatrix * transformation * vec4(inputPosition, 1.0);
	textureCoords = vec2(inputPosition.x, 1 - inputPosition.y);
}