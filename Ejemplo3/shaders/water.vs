#version 400

layout (location = 0) in vec3 inputPosition;
layout (location = 1) in vec3 inputNormal;
layout (location = 2) in vec2 inputTexCoord;
layout (location = 3) in vec3 inputTangent;
layout (location = 4) in vec3 inputBinormal;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix = mat4(1.0);
uniform mat4 projectionMatrix = mat4(1.0);
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

const float tiling = 4.0;

void main() {
	gl_ClipDistance[0] = 1;

	vec4 worldPosition = modelMatrix * vec4(inputPosition, 1.0);
	clipSpace = projectionMatrix * viewMatrix * worldPosition;
	gl_Position = clipSpace;
	textureCoords = vec2(inputPosition.x / 2.0 + 0.5, inputPosition.z / 2.0 + 0.5) * tiling;
	toCameraVector = cameraPosition - worldPosition.xyz;
	fromLightVector = worldPosition.xyz - lightPosition;
}