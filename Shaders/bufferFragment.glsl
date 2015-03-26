#version 150 core

uniform sampler2D diffuseTex;
uniform int useTexture;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
} IN;

out vec4 gl_FragColor[2];

void main(void) {
	
	vec3 normal = IN.normal;
	gl_FragColor[0] = texture2D(diffuseTex, IN.texCoord);
	gl_FragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
}