/*
	Class: Renderer.
		This class is in charge of rendering all the objects that are going to be seen on screen.
		This class inherits from the OGLRenderer class that was given to us in the nclgl framework.
*/


#pragma once
#include "../nclgl_coursework/OGLRenderer.h"
#include "../nclgl_coursework/Camera.h"
#include "../nclgl_coursework/SceneNode.h"
#include "../nclgl_coursework/HeightMap.h"
#include "Tree.h"
#include "TextMesh.h"
#include "../nclgl_coursework/Frustum.h"
#include "../nclgl_coursework/ParticleEmitter.h"

//The size of the shadow
#define SHADOWSIZE 2048

class Renderer: public OGLRenderer {
public:
	/* My guess is, that this public methods don't need explanation, do they? */
	Renderer(Window &parent);
	virtual ~Renderer(void);


	virtual void UpdateScene(float msec);
	virtual void RenderScene();

protected:
	//Methods explained on the .cpp class
	void ModifyLightPosition(float up_down, float right_left);
	void DrawNode(SceneNode *n, bool shadow_testing);
	void DrawHeightMap(bool shadow_testing);
	void DrawSkyBox();
	void DrawWater(bool shadow_testing);
	//This methods will be left here in case everything breaks :(
	void DrawShadowScene();
	void DrawCombinedScene();

	/*
		The regios labeled newmethodstuff (or stuff like that) where created in attempt to get deffered lightning working,
		BUT, because I couldn'tr actually make it work I decided to drop it off, but let the code stay here just in case I someday 
		come back to it. 
		UPDATE: The main reason why it didn't worked may be that I created everything and let the lights for the end, when it should
			be the other way (get the defferred lights working, then create everything else around that way. This means my code is a bit
			unflexible for changes. 
	*/
#pragma region newmethodsandstuff
	//// This methods will take charge o the multiple lighting stuff :) (i hope ¬¬)
	//void FillBuffers();
	//void DrawPointLights();
	//void CombineBuffers();
	//void GenerateScreenTexture(GLuint &into, bool depth = false);
	//void DrawNewNodes();
	//Shader* sceneShader;
	//Shader*pointLightShader;
	//Shader* combineShader;
	//Light* night_lights;
	//OBJMesh* sphere;
	//Mesh* quad2;

	//GLuint bufferFBO; //FBO for our G-buffer pass
	//GLuint bufferColourTex; // Albedo goes here
	//GLuint bufferNormalTex; //Normals go here
	//GLuint bufferDepthTex; //Depth goes here

	//GLuint pointLightFBO; //FBO for our lighting pass
	//GLuint lightEmissiveTex; //Store emissive lighting
	//GLuint lightSpecularTex; //Store specular lighting
#pragma endregion

#pragma region newstuffofthenewstuff
	/*Shader* new_tree_Shader;
	Shader* new_highmap_shader;*/
#pragma endregion

	//Methods explained on the .cpp class
	void DrawMessage();
	void BuildNodeLists(SceneNode *from);
	void DrawNodes(bool shadow_testing);
	void ClearNodeLists();
	void DrawParticles();

	void GenerateShadowTexture();
	void setShadowTesting();
	void prepareCombinedScene();
	void DrawParticle(ParticleEmitter* p);
	void UpdateFireworks(ParticleEmitter *p, float limit);
	void UpdateParticles(float msec);
	void UpdateRain(float msec);

	void UpdateLight();

	/*
		Draws the passed in line of text, at the passed in position. Can have an optional font size, and
		whether to draw it in orthographic or perspective mode. This method is taken from one of the extra tutorials.
	*/
	void DrawText(const std::string &text, const Vector3 &position, const float size = 10.0f, const bool perspective = false);

	//Shaders, their names should be self explanatory
	Shader* lightShader;
	Shader* skyBoxShader;
	Shader* reflectShader;
	Shader* treeShader;
	Shader* shadowShader;
	Shader* textShader;
	Shader* particleShader;

	//classes, their names should be self - explanatory
	HeightMap* heightMap;
	Mesh* quad;
	Light* light;
	Camera *camera;
	Font* basicFont;
	SceneNode* root;
	SceneNode* rain;
	SceneNode* torches;
	SceneNode* grass;
	Frustum frameFrustum;
	
	//particles
	ParticleEmitter* emitter;
	ParticleEmitter* torch1;
	ParticleEmitter* torch2;
	ParticleEmitter* torch3;
	ParticleEmitter* torch4;
	
	//names of gl elements, names should be self explanatory
	GLuint cubeMap;
	GLuint shadowTex;
	GLuint shadowFBO;
	
	Matrix4 shadowModelViewProj;
	
	float waterRotate; // to rotate water
	float msec_passed; // the miliseconds passed since las update
	int framerate; // the frames that has passed in the last miliseconds (more explanations to come)
	int tree_number; // the randomly generated number
	bool day; //true is day, false is night
	float height_water; // represents the height of the water
	bool rain_flag;//sets wether its raining or not

	float fogValue;
	
	//The nodes that are inside the frustum
	vector<SceneNode*> visibleNodes;
};