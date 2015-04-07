#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;
uniform sampler2DShadow shadowTex;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float lightRadius;
uniform int activateFog;
uniform float fogValue;

in Vertex{
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
	vec3 incident = normalize(IN.worldPos - cameraPos);
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist/lightRadius, 0.2, 1.0);
	vec4 reflection = texture(cubeTex, reflect(incident, normalize (IN.normal)));
	
	float shadow = 1.0;
	
	if(IN.shadowProj.w > 0.0){
		shadow = textureProj(shadowTex, IN.shadowProj);
		//shadow = vec4(1,0,0,1);
	}
	
	shadow += 1.f;
	
	
	if(activateFog < 1){
		gl_FragColor.rgb = (lightColour.rgb*diffuse.rgb*atten*shadow) * (diffuse.rgb + reflection.rgb);
		gl_FragColor.a = lightColour.a * diffuse.a;
	} else {
	// fog explained on the mainFragment.glsl file
		float dist2 = gl_FragCoord.z / gl_FragCoord.w;
		float fogFactor = (1800 - dist2)/(1800 - 100);
		fogFactor = fogFactor/fogValue;
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
		 vec4 temp_colour;
		        
		
		temp_colour.rgb = (lightColour.rgb*diffuse.rgb*atten*shadow) * (diffuse.rgb + reflection.rgb);
		temp_colour.a = lightColour.a * diffuse.a;
		
		
		
		gl_FragColor = mix(vec4(0.5, 0.5, 0.5, 1), temp_colour, fogFactor);
	}	
}