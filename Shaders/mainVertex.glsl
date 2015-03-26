#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform vec4 nodeColour;
in  vec3 position;
in  vec2 texCoord;

in vec3 normal;

out Vertex {
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj;
} OUT;

void main(void)	{
	OUT.colour = nodeColour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;
	
	OUT.shadowProj = (textureMatrix * vec4(position+(normal*1.5), 1));
	
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
}