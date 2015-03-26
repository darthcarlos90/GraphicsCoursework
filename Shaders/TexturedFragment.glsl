#version 150 core

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
	float height;
} IN;

out vec4 gl_FragColor;

void main(void){
	//high mountains get painted with snow
	if(IN.height >= 240.0f){
		gl_FragColor = IN.colour;
	} else
		gl_FragColor = texture(diffuseTex, IN.texCoord);
} 