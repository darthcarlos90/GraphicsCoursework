#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2DShadow shadowTex;

uniform int useTexture;
uniform int nodeLevel;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform int activateFog;



in Vertex{
	vec2 texCoord;
	vec4 colour;
	vec3 normal;
	vec3 worldPos;
	vec4 shadowProj;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 diffuse;
	if(useTexture > 0){
		if(nodeLevel != 0){
			diffuse = texture(diffuseTex, IN.texCoord) * IN.colour;
		}else {
			diffuse = texture(diffuseTex, IN.texCoord);
		}
	} else {
		diffuse = IN.colour;
	}
		
	
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
		shadow = textureProj(shadowTex, IN.shadowProj);
	}
	
	 lambert *= shadow;
	 //if fog is activated
	if(activateFog > 0){
		 float fog_distance = gl_FragCoord.z / gl_FragCoord.w;
		 /*
			Here we are using a fog factor of linerar fog. Here, we input
			at what relative distance from the camera we want to start the fog, and at what 
			distance we want to end the fog, afterwards we calculate the fogFactor using the
			fog distance (the distance from the camera) to create the fog factor. This code
			(which is repeated in other shaders) is based on the code provided in the 
			tutorial located at: http://in2gpu.com/2014/07/22/create-fog-shader/
		 */
		float fogFactor = (1800 - fog_distance)/(1800 - 100);
		
		//clamping the fog factor
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
		
		//create a temporal colour to save all the colour from the shadow operations
		vec4 temp_colour;
		vec3 colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.33;
		temp_colour = vec4(colour * atten * lambert, diffuse.a);
		temp_colour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
		
		//the final output colour is given by the function mix, mixing the colour of the fog 
		//(gray), the temp_colour with all our calculations, and the fog factor
		gl_FragColor = mix(vec4(0.5, 0.5, 0.5, 1), temp_colour, fogFactor);
	 }else {
		//if we are not using fog, output as usual
		vec3 colour = (diffuse.rgb * lightColour.rgb);
		colour += (lightColour.rgb * sFactor) * 0.33;
		gl_FragColor = vec4(colour * atten * lambert, diffuse.a);
		gl_FragColor.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
	 }
	
}