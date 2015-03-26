#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2DShadow shadowTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform int activateFog;

in Vertex {
	vec3 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	
	vec3 incident = normalize(lightPos - IN.worldPos);
	float lambert = max(0.0, dot(incident, IN.normal));
	
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float rFactor = max(0.0, dot(halfDir, IN.normal));
	float sFactor = pow(rFactor, 50.0);
	
	float shadow = 1.0;
	
	if(IN.shadowProj.w > 0.0){
		if ((IN.shadowProj.x / IN.shadowProj.w) >= 0.0f
			&& (IN.shadowProj.x / IN.shadowProj.w) <= 1.0f
			&& (IN.shadowProj.y / IN.shadowProj.w) >= 0.0f
			&& (IN.shadowProj.y / IN.shadowProj.w) <= 1.0f)
		{
			shadow = textureProj(shadowTex, IN.shadowProj);
		}
	}
	
	lambert *= shadow; 
	if(activateFog < 1){
		vec3 colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.33;
		gl_FragColor = vec4(colour * atten * lambert, diffuse.a);
		
		gl_FragColor.rgb += (diffuse.rgb * lightColour.rgb)*0.1 ;
	} else {
		//fog operations explained in the MainFragment.glsl file
		float dist2 = gl_FragCoord.z / gl_FragCoord.w;
		float fogFactor = (1800 - dist2)/(1800 - 100);
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
		 vec4 temp_colour;
		
		vec3 colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.33;
		temp_colour = vec4(colour * atten * lambert, diffuse.a);
		temp_colour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
		
		gl_FragColor = mix(vec4(0.5, 0.5, 0.5, 1), temp_colour, fogFactor);
	}
	
}