#include "Renderer.h"
#include "Sphere.h"
#include "Branch.h"
#include "Grass.h"
//Constant that represents how much the water will increase with rain
static const float WATER_INCREASE = 0.02f;

//Constant that represents how much will the sun move
static const float LIGHT_MOVEMENT = 4.0f;

//Constant that represents the colour of the light
static const Vector4 LIGHT_COLOUR(0.9f, 0.9f, 1.0f, 1);

//Constructor
Renderer::Renderer(Window &parent) : OGLRenderer(parent){
	Tree::CreateCylinder();
	Tree::CreateSphere();
	Branch::CreateCylinder();
	Sphere::CreateSphere();
	Grass::CreateGrass();

	//As I said, code that ended up not working :(
#pragma region deferedstuff
	/*quad2 = Mesh::GenerateQuad();
	sphere = new OBJMesh();
	if(!sphere->LoadOBJMesh("Meshes/Sphere.obj"))
		return;
	
	sceneShader = new Shader("Shaders/BumpVertex.glsl", "Shaders/bufferFragment.glsl");

	if(!sceneShader->LinkProgram())
		return;

	combineShader = new Shader("Shaders/combinevert.glsl", "Shaders/combinefrag.glsl");
	if(!combineShader->LinkProgram())
		return;

	pointLightShader = new Shader("Shaders/pointlightvertex.glsl", "Shaders/pointlightfragment.glsl");
	if(!pointLightShader->LinkProgram())
		return;*/

#pragma region newstuffofthenewstuff
	/*new_tree_Shader = new Shader("Shaders/newVert.glsl", "Shaders/newBufferFragment.glsl");
	new_highmap_shader = new Shader("Shaders/newVertH.glsl", "Shaders/newBufferFragment2.glsl");*/
#pragma endregion


	//GLenum buffers[2];
	//buffers[0] = GL_COLOR_ATTACHMENT0;
	//buffers[1] = GL_COLOR_ATTACHMENT1;

	////GENERATE OUR SCENE DEPTH TEXTURE ..
	//GenerateScreenTexture(bufferDepthTex, true);
	//GenerateScreenTexture(bufferColourTex);
	//GenerateScreenTexture(bufferNormalTex);
	//GenerateScreenTexture(lightEmissiveTex);
	//GenerateScreenTexture(lightSpecularTex);

	////And now attach them to our FBOs
	//glGenFramebuffers(1, &bufferFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferNormalTex, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	//glDrawBuffers(2, buffers);

	//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	return;
	//glGenFramebuffers(1, &pointLightFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	//glDrawBuffers(2, buffers);

	//if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	return;

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
#pragma endregion

	// here starts the region of the stuff being used
#pragma region otherstuff
	camera = new Camera(0.0f, 0.0f, Vector3(RAW_WIDTH*HEIGHTMAP_X / 2.0f, 500.0f, RAW_WIDTH*HEIGHTMAP_X));
	heightMap = new HeightMap("Textures/terrain.raw");
	quad = Mesh::GenerateQuad();

	light = new Light(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f) , 1000.0f,
		(RAW_HEIGHT*HEIGHTMAP_Z/2.0f)),
		LIGHT_COLOUR, 
		(RAW_WIDTH*HEIGHTMAP_X));
//	night_lights = new Light[5];
	
	
	
	height_water = 0.0f;
	
	lightShader = new Shader("Shaders/PerPixelVertex.glsl", "Shaders/PerPixelFragment.glsl");
	skyBoxShader = new Shader("Shaders/skyboxVertex.glsl", "Shaders/skyboxFragment.glsl");
	treeShader = new Shader("Shaders/mainVertex.glsl", "Shaders/mainFragment.glsl");
	shadowShader = new Shader("Shaders/shadowVert.glsl", "Shaders/shadowFrag.glsl");
	reflectShader = new Shader("Shaders/PerPixelVertex.glsl", "Shaders/reflectFragment.glsl");
	textShader = new Shader("Shaders/TexturedVertex.glsl", "Shaders/TexturedFragment.glsl");
	particleShader = new Shader("Shaders/simplevertex.glsl", "Shaders/simplefragment.glsl", "Shaders/geometry.glsl");

	if(!lightShader->LinkProgram() ||!reflectShader->LinkProgram() || 
		!skyBoxShader->LinkProgram() || !treeShader->LinkProgram() ||
		!textShader->LinkProgram() || !particleShader->LinkProgram()) 
		return;
	
	GenerateShadowTexture();
	basicFont = new Font(SOIL_load_OGL_texture("Textures/tahoma.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_COMPRESS_TO_DXT),16, 16);
	quad->SetTexture(SOIL_load_OGL_texture("Textures/water.TGA", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	heightMap->SetTexture(SOIL_load_OGL_texture("Textures/sand_texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
	cubeMap = SOIL_load_OGL_cubemap(
		"Textures/rusted_west.jpg", "Textures/rusted_east.jpg",
		"Textures/rusted_up.jpg", "Textures/rusted_down.jpg", 
		"Textures/rusted_south.jpg", "Textures/rusted_north.jpg",
		SOIL_LOAD_RGB, 
		SOIL_CREATE_NEW_ID, 0);
	
	if(!heightMap->GetTexture() || !quad->GetTexture() || !cubeMap)
		return;

	
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(quad->GetTexture(), true);
	
	framerate = 0;
	msec_passed = 0.0f;

	waterRotate = 0.0f;
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width/ (float)height, 45.0f);
	
	//Set the sceneNodes of the trees
	root = new SceneNode();
	Vector3 tree_location =  Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 90.0f, (RAW_HEIGHT*HEIGHTMAP_Z/2.0f));
	root->AddChild(new Tree(5, tree_location));
	tree_number = rand() % 10 + 1;
	for(int i = 0; i < tree_number; i++){
		//create a random location for each tree
		Vector3 randlocation = Vector3(rand() % RAW_HEIGHT*HEIGHTMAP_X, 75.0f, rand() % RAW_HEIGHT*HEIGHTMAP_Z);
		//create a random number of levels for the tree
		int random_levels = rand() % 5 + 1;
		//create a random shape for the tree
		float random_shape = (rand() % 201) / 100.0f;
		//add that tree to the root
		root->AddChild(new Tree(random_levels, randlocation, random_shape));
	}

	//Create the particles
	//this particle is the fireworks
	emitter = new ParticleEmitter(Vector3(tree_location.x, 200.0f, tree_location.z), false);

	//this particles are the fire of the torches
	torch1 = new ParticleEmitter(Vector3(100.0f,120.0f,100.0f), true);
	torch2 = new ParticleEmitter (Vector3(100.0f,120.0f, (RAW_HEIGHT*HEIGHTMAP_Z) - 100.0f), true);
	torch3 = new ParticleEmitter(Vector3((RAW_HEIGHT*HEIGHTMAP_X) - 100.0f, 120.0f, 100.0f), true);
	torch4 = new ParticleEmitter(Vector3((RAW_HEIGHT*HEIGHTMAP_X) - 100.0f, 120.0f, (RAW_HEIGHT*HEIGHTMAP_Z) - 100.0f), true);
	
	/*
		Oops, some of the not working code slipped out of the region, sorry.
	*/
	//night_lights[0] = *light;
	//night_lights[0] = Light(torch1->GetLocation(), LIGHT_COLOUR, 10.0f);
	/*night_lights[TORCH2] = Light(torch2->GetLocation(), LIGHT_COLOUR, 10.0f);
	night_lights[TORCH3] = Light(torch3->GetLocation(), LIGHT_COLOUR, 10.0f);
	night_lights[TORCH4] = Light(torch4->GetLocation(), LIGHT_COLOUR, 10.0f);
	night_lights[MAIN_TORCH] = Light(emitter->GetLocation(), LIGHT_COLOUR, 10.0f);*/

	// The rain sceneNode
	rain = new SceneNode();
	for(int i = 0; i < 1000; i++){
		//Create the rain drop at a random space location, to make believe there is rain uniformily falling
		Vector3 randlocation = Vector3(rand() % RAW_HEIGHT*HEIGHTMAP_X, (rand() % 4001)/ 1.0f, rand() % RAW_HEIGHT*HEIGHTMAP_Z);
		rain->AddChild(new Sphere(randlocation, 2000.0f, Vector4(0.0f, 0.3f, 1.0f, 0.7f)));
	}

	//The torches trunks
	torches = new SceneNode();
	torches->AddChild(new Branch(Vector3(100.0f, 60.0f, 100.0f)));
	torches->AddChild(new Branch(Vector3(100.0f ,60.0f, (RAW_HEIGHT*HEIGHTMAP_Z) - 100.0f)));
	torches->AddChild(new Branch(Vector3((RAW_HEIGHT*HEIGHTMAP_X) - 100.0f, 60.0f, 100.0f)));
	torches->AddChild(new Branch(Vector3((RAW_HEIGHT*HEIGHTMAP_X) - 100.0f, 60.0f, (RAW_HEIGHT*HEIGHTMAP_Z) - 100.0f)));

	//grass, sabe as the others
	grass = new SceneNode();
	grass->AddChild(new Grass(tree_location));
	for(int i = 0; i < 300; i++){
		Vector3 location = Vector3(rand()%RAW_HEIGHT*HEIGHTMAP_X, 0.0f, rand()%RAW_HEIGHT*HEIGHTMAP_Z);
		grass->AddChild(new Grass(location));
	}

#pragma endregion

	//some enableing

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	
	//flags for day, raining, and init of course
	day = true;
	rain_flag = true;
	init = true;
}

//destructor
Renderer::~Renderer(void)	{
	glDeleteTextures(1, &shadowTex);
	glDeleteFramebuffers(1, &shadowFBO);
	
	delete camera;
	delete root;
	delete rain;
	delete heightMap;
	delete light;
	delete skyBoxShader;
	delete quad;
	
	delete reflectShader;
	delete treeShader;
	delete shadowShader;
	delete lightShader;
	currentShader = 0;
	delete basicFont;
	delete emitter;
	
	delete torch1;
	delete torch2;
	delete torch3;
	delete torch4;

	delete torches;

	delete grass;

	//delete [] night_lights;

	Tree::DeleteCylinder();
	Tree::DeleteSphere();
	Branch::DeleteCylinder();
	Sphere::DeleteSphere();
	Grass::DeleteGrass();

	//New stuff
	/*delete sceneShader;
	delete combineShader;
	delete pointLightShader;
	delete sphere;
	delete quad2;*/
	/*glDeleteTextures(1, &bufferColourTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &pointLightFBO);*/



}
//comented method that is not being used.
//void Renderer::GenerateScreenTexture(GLuint &into, bool depth){
//	glGenTextures(1, &into);
//	glBindTexture(GL_TEXTURE_2D, into);
//
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//	glTexImage2D(GL_TEXTURE_2D, 0,
//		depth? GL_DEPTH_COMPONENT24:GL_RGBA8,
//		width, height, 0,
//		depth? GL_DEPTH_COMPONENT : GL_RGBA,
//		GL_UNSIGNED_BYTE, NULL);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

/*
	Method: UpdateScene
		This method updates everything so it can be rendered on screen.
	Params:
		msec: The milliseconds passed since las update.
*/
void Renderer::UpdateScene(float msec){
	//All the stuff here should be self expalanatory, the methods
	//are explained down anyways
	UpdateParticles(msec);
	camera->UpdateCamera(msec);
	UpdateLight();

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width/(float)height, 45.0f);
	//Que dice mi mama que siempre si hay que poner esto aqui ¬¬ (sorry for my little spanish comment)
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);

	root->Update(msec);
	torches->Update(msec);
	grass->Update(msec);
	waterRotate += msec/1000.0f;
	msec_passed = msec;
	framerate++;
	UpdateRain(msec);
}
/*
	Method: UpdateRain
		This method basically makes the illusion of the scene getting flooded and of the 
		water beign evaporated.
	Params:
		msec: The milliseconds passed.
*/
void Renderer::UpdateRain(float msec){
	//if its raining
	if(rain_flag){
		//if the water hasnt reached its limit
		if(height_water < (256 * HEIGHTMAP_Y / 3.0f)){
			//raise water
			height_water += WATER_INCREASE;
			rain->Update(msec);
		}else {
			//else, stop raining
			rain_flag = false;
		}
	} else {
		//if the scene is flooded and its daylight, evaporate
		if(height_water > 0.0f){
			if(day)
				height_water -= WATER_INCREASE;
		} else {
			//if finish evaporating, start raining
			rain_flag = true;
		}
	}
}

/*
	Method: UpdateParticles
		This method updates the particles to their position.
	Params:
		msec: The milliseconds passed.
*/
void Renderer::UpdateParticles(float msec){
	emitter->Update(msec);
	torch1->Update(msec);
	torch2->Update(msec);
	torch3->Update(msec);
	torch4->Update(msec);
}

/*
	Method: UpdateLight.
		This method moves the light to simulate day and night, when the light reaches one point, it is moved back
		to a starting point. Also depending on the position of the light, it states the day flag to either true or
		false.
*/
void Renderer::UpdateLight(){
	if(light->GetPosition().x < (RAW_HEIGHT*HEIGHTMAP_X) + light->GetRadius())
		ModifyLightPosition(LIGHT_MOVEMENT, 0.0f);
	else light->SetPosition(Vector3(-(RAW_HEIGHT*HEIGHTMAP_X + light->GetRadius()), light->GetPosition().y, light->GetPosition().z));

	if(light->GetPosition().x > ((RAW_HEIGHT*HEIGHTMAP_X)/2.0f) + (light->GetRadius()/2.0f) ||
		light->GetPosition().x < -(light->GetRadius()))
		day = false;
	else day = true;
}

/*
	Method: BuildNodeLists.
		This method pushes into a vector the amount of nodes that are inside the frustum.
		This method calls itself recursively to traverse the whole tree.
	Params:
		from: The root node of the scene tree.
*/
void Renderer::BuildNodeLists(SceneNode* from){
	if(frameFrustum.InsideFrutusm(*from)){
		visibleNodes.push_back(from);
	}

	for(vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i != from->GetChildIteratorEnd(); i++){
		BuildNodeLists((*i));
	}
}

/*
	Method: DrawNodes
		This method traverses the vector and draws the node.
	Params:
		shadow_testing: this variable is explained in later methods.
*/
void Renderer::DrawNodes(bool shadow_testing){
	for(vector<SceneNode*>::const_iterator i = visibleNodes.begin(); i != visibleNodes.end(); i++){
		DrawNode((*i), shadow_testing);
	}
}

/*
	Method: ClearNodeLists
		This method clears the node list so that elements are not repeated the nex frame.
*/
void Renderer::ClearNodeLists(){
	visibleNodes.clear();
}

/*
	Method: RenderScene.
		This method calls the methods necessary to render the scene on screen.
*/
void Renderer::RenderScene()	{
	//first build the scene nodes
	BuildNodeLists(root);
	BuildNodeLists(torches);
	BuildNodeLists(grass);
#pragma region oldway
	//if its raining, also build the list of drops
	if(rain_flag)
		BuildNodeLists(rain);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	DrawSkyBox(); //Draw Skybox
	
	DrawMessage(); // the message
	DrawShadowScene(); //First render pass
	DrawCombinedScene(); //Second render pass
	
	SwapBuffers();	// swapp the buffers
	ClearNodeLists(); //clear the node list
#pragma endregion
#pragma region newway
	/*glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	DrawWater(false);
	FillBuffers();
	DrawPointLights();
	CombineBuffers();
	viewMatrix = camera->BuildViewMatrix();
	
	SwapBuffers();*/
#pragma endregion
	

}
//comented method not being used
//void Renderer::DrawNewNodes(){
//	for(vector<SceneNode*>::const_iterator i = visibleNodes.begin(); i != visibleNodes.end(); i++){
//		SceneNode *n = (*i);
//		if(n->GetMesh()){
//			glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
//			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
//			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "nodeLevel"), (int)n->getLevel());
//			Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
//			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
//			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "drawingNode"), 1);
//			n->Draw();
//		}
//	}
//}

//comented method not used
//draw everything that is not light
//void Renderer::FillBuffers(){
//	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
//	glClearColor(0.5, 0.5, 0.5, 1);
//	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width/(float)height, 45.0f);
//
//	DrawSkyBox(); //Draw Skybox
//	
//
//	SetCurrentShader(sceneShader);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
//
//	modelMatrix.ToIdentity();
//	UpdateShaderMatrices();
//
//	heightMap->Draw();
//	DrawNewNodes();
//
//	
//
//	glUseProgram(0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//}

//comented method not used
//void Renderer::DrawPointLights(){
//	SetCurrentShader(pointLightShader);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
//
//	glClearColor(0, 0, 0, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glBlendFunc(GL_ONE, GL_ONE);
//
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), 3);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), 4);
//
//
//	glActiveTexture(GL_TEXTURE3);
//	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
//
//	glActiveTexture(GL_TEXTURE4);
//	glBindTexture(GL_TEXTURE_2D, bufferNormalTex);
//
//	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "camerPos"), 1, (float*)&camera->GetPosition());
//	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f/width, 1.0f/height);
//	glDisable(GL_DEPTH_TEST);
//
//	for(int x = 0; x < 2; x++){
//		Light &l = night_lights[x];
//		float radius = l.GetRadius();
//		SetShaderLight(l);
//
//		modelMatrix = 
//				Matrix4::Translation(l.GetPosition()) * 
//				Matrix4::Scale(Vector3(radius, radius, radius));
//		UpdateShaderMatrices();
//
//		float dist = (l.GetPosition() - camera->GetPosition()).Length();
//		if(dist < radius) //camera is inside the light volume!
//			glCullFace(GL_FRONT);
//		else
//			glCullFace(GL_BACK);
//		sphere->Draw();
//	}
//
//	glEnable(GL_DEPTH_TEST);
//	glCullFace(GL_BACK);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glUseProgram(0);
//}
//
//void Renderer::CombineBuffers(){
//	
//	SetCurrentShader(combineShader);
//
//	projMatrix = Matrix4::Orthographic(-1,1,1,-1,-1,1);
//	UpdateShaderMatrices();
//
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 2);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), 3);
//	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specularTex"), 4);
//
//	glActiveTexture(GL_TEXTURE2);
//	glBindTexture(GL_TEXTURE_2D, bufferColourTex);
//	
//	glActiveTexture(GL_TEXTURE3);
//	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);
//
//	glActiveTexture(GL_TEXTURE4);
//	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);
//
//	quad2->Draw();
//
//	glUseProgram(0);
//}

/*
	Method: DrawSkyBox
		This method is in charge of drawing the skybox.
*/
void Renderer::DrawSkyBox(){
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyBoxShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

/*
	Method: DrawParticles
		This method draws the particles on screen.
*/
void Renderer::DrawParticles(){
	SetCurrentShader(particleShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 4);
	//update the fireworks position
	UpdateFireworks(emitter, 1000.0f);
	
	//if it is not raining, lit the torches
	if(!rain_flag){
		DrawParticle(torch1);
		DrawParticle(torch2);
		DrawParticle(torch3);
		DrawParticle(torch4);
	}
	glUseProgram(0);
}

/*
	Method: DrawParticle
		This method is in charge of updating the particles information.
	Params:
		p: The particle beign updated.
*/
void Renderer::DrawParticle(ParticleEmitter* p){
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), p->GetParticleSize());
	
	p->SetParticleSize(8.0f);
	p->SetParticleVariance(1.0f);
	p->SetLaunchParticles(32.0f);
	p->SetParticleLifetime(2000.0f);
	p->SetParticleSpeed(0.05f);
	p->SetDirection(Vector3(0,5,0));
	UpdateShaderMatrices();

	p->Draw();
}

/*
	Method: UpdateFireworks
		This method updates the fireworks path.
	Params:
		p: The particle beign updated.
		limit: The limit of the trayectory.
*/
void Renderer::UpdateFireworks(ParticleEmitter *p, float limit){
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "particleSize"), p->GetParticleSize());
	//if it hasnt reached the limit, move
	if(p->GetLocation().y < limit){
		p->updateLocation(Vector3(0.0f, 10.0f, 0.0f));
		p->SetParticleSize(8.0f);
		p->SetParticleVariance(1.0f);
		p->SetLaunchParticles(32.0f);
		p->SetParticleLifetime(2000.0f);
		p->SetParticleSpeed(0.05f);
		p->SetDirection(Vector3(0,-5,0));
		//if it reached the top, stop and shine : D
	} else {
		p->SetParticleSize(8.0f);
		p->SetParticleVariance(1.0f);
		p->SetLaunchParticles(32.0f);
		p->SetParticleLifetime(2000.0f);
		p->SetParticleSpeed(0.05f);
		p->SetDirection(Vector3(0,0,0));
	}
	UpdateShaderMatrices();
	p->Draw();
}

/*
	Method: DrawShadowScene.
		The first pass of the light calculations, draw to the shadow buffer
		from the lights perspective.
*/
void Renderer::DrawShadowScene(){
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	projMatrix = Matrix4::Orthographic(1, 10000, 1000, -1000, 1000, -1000);
	DrawHeightMap(true);
	
	DrawNodes(true);

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/*
	Method: DrawCombinedScene.
		Second pass of the light calculation, draw from the cameras perspective and use the 
		depths calculated from the first pass to draw the shadow. (Sorry if I explained something wrong,
		but finishing commenting at 5 am is not fun :( ).
*/
void Renderer::DrawCombinedScene(){
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width/ (float)height, 45.0f);
	viewMatrix = camera->BuildViewMatrix();
	//if it is night, draw the particles
	if(!day){
		DrawParticles();
	} else {
		//reste fireworks location
		emitter->SetLocation(Vector3((RAW_HEIGHT*HEIGHTMAP_X / 2.0f), 200.0f, (RAW_HEIGHT*HEIGHTMAP_Z/2.0f)));
	}
	DrawHeightMap(false);
	DrawWater(false);
	DrawNodes(false);
}

/*
	Method: DrawHeightMap
		Draws The heightmap.
	Params:
		shadow_testing: if true, use the shaders for the first pass of the shadow rendering, if false, it means
			we are rendering to screen.
*/
void Renderer::DrawHeightMap(bool shadow_testing){
	if(shadow_testing){
		setShadowTesting();	
		modelMatrix.ToIdentity();
		UpdateShaderMatrices();
	}else {
		SetCurrentShader(lightShader);
		prepareCombinedScene();
		modelMatrix.ToIdentity();
		textureMatrix.ToIdentity();
		UpdateShaderMatrices();

		Matrix4 tempMatrix = shadowModelViewProj * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowModelViewProj")  ,1,false, (float*)&tempMatrix);
		//if it is night and not raining, show the fog
		if(!day){
			if(!rain_flag)
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 1);
		} else {
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 0);
		}
	}

	heightMap->Draw();
	glUseProgram(0);
}

/*
	Method: DrawWater.
		Method that draws the water and its reflection.
	Params:
		shadow_testing: already explained.
*/
void Renderer::DrawWater(bool shadow_testing){
	if(shadow_testing){
		setShadowTesting();
	} else {
		SetCurrentShader(reflectShader);
		prepareCombinedScene();
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
		float heightX = (RAW_WIDTH*HEIGHTMAP_X/ 2.0f);
		float heightY = height_water;
		float heightZ = (RAW_HEIGHT*HEIGHTMAP_Z / 2.0f);
		modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) * 
			Matrix4::Scale(Vector3(heightX, 1, heightZ)) * 
			Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));
		textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * 
			Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

		
		Matrix4 tempMatrix = shadowModelViewProj * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowModelViewProj")  ,1,false, (float*)&tempMatrix);
		if(!day){
			if(!rain_flag)
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 1);
		} else {
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 0);
		}

		UpdateShaderMatrices();
	}
	quad->Draw();
	glUseProgram(0);
}

/*
	Method: DrawNode
		It draws the node using the specified shader in the input shadow testing.
	Params:
		p: The node to be drawn.
*/
void Renderer::DrawNode(SceneNode* n, bool shadow_testing){
	if(n->GetMesh()){
		if(shadow_testing){
			setShadowTesting();
		}else {
			SetCurrentShader(treeShader);
			prepareCombinedScene();
			modelMatrix.ToIdentity();
			textureMatrix.ToIdentity();
			UpdateShaderMatrices();

			glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "nodeLevel"), (int)n->getLevel());
		}

		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		if(!day){
			if(!rain_flag)
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 1);
		} else {
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "activateFog"), 0);
		}

		n->Draw();
	}
	glUseProgram(0);
}

/*
	Method: GenerateShadowTexture.
		Generates the shadow texture and the shadow buffer which will be used for 
		calculating our shadows.
*/
void Renderer::GenerateShadowTexture(){
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
}

/*
	Method: setShadowTesting
		makes the shadow testing, BUT, the shader never wanted to work and never
		turned back to fix it. So another shader is used, it takes more time, but works
		at least :(
*/
void Renderer::setShadowTesting(){
	//SetCurrentShader(shadowShader); --WHY THIS!? :(
	SetCurrentShader(lightShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(),  Vector3(2000,0,2000));
	shadowModelViewProj = biasMatrix*(projMatrix*viewMatrix);
	UpdateShaderMatrices();
}

/*
	Method: prepareCombinedScene.
		Prepares the uniforms for the combined scene.
*/
void Renderer::prepareCombinedScene(){
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i (glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i (glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
}

/*
	Method: ModifyLightPosition
		Modifies the light position.
	Params:
		up_down: positive to move up, negative to move down
		right_left : positive right, negative left
*/
void Renderer::ModifyLightPosition(float up_down, float right_left){
	light->SetPosition(light->GetPosition() + (Vector3(up_down, right_left, 0.0)));
}

/*
	Method: DrawMessage.
		Draws a message into the screen.
*/
void Renderer::DrawMessage(){
    SetCurrentShader(textShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	ostringstream str;
	ostringstream str2;
	ostringstream str3;
	float frames = (float)framerate/(msec_passed /1000.0f);
	str << "Framerate: " << frames << std::endl;
	DrawText(str.str(), Vector3(0,0,0), 16.0f);
	float size_used = (float)root->getMemSizeTree()/1000000.0f;
	str2 << "Memory Used: " << size_used << "Mb";
	DrawText(str2.str(), Vector3(0,16.0f,0),16.0f);
	str3 << "Trees Created: " << (tree_number + 1);
	DrawText(str3.str(), Vector3(0, 32.0f, 0), 16.0f);
	glUseProgram(0);
	framerate = 0;
}

/*
Draw a line of text on screen. If we were to have a 'static' line of text, we'd
probably want to keep the TextMesh around to save processing it every frame, 
but for a simple demonstration, this is fine...
*/
void Renderer::DrawText(const std::string &text, const Vector3 &position, const float size, const bool perspective)	{
	//Create a new temporary TextMesh, using our line of text and our font
	TextMesh* mesh = new TextMesh(text,*basicFont);

	//This just does simple matrix setup to render in either perspective or
	//orthographic mode, there's nothing here that's particularly tricky.
	if(perspective) {
		modelMatrix = Matrix4::Translation(position) * Matrix4::Scale(Vector3(size,size,1));
		viewMatrix = camera->BuildViewMatrix();
		projMatrix = Matrix4::Perspective(1.0f,10000.0f,(float)width / (float)height, 45.0f);
	}
	else{	
		//In ortho mode, we subtract the y from the height, so that a height of 0
		//is at the top left of the screen, which is more intuitive
		//(for me anyway...)
		modelMatrix = Matrix4::Translation(Vector3(position.x,height-position.y, position.z)) * Matrix4::Scale(Vector3(size,size,1));
		viewMatrix.ToIdentity();
		projMatrix = Matrix4::Orthographic(-1.0f,1.0f,(float)width, 0.0f,(float)height, 0.0f);
	}
	//Either way, we update the matrices, and draw the mesh
	UpdateShaderMatrices();
	mesh->Draw();

	delete mesh; //Once it's drawn, we don't need it anymore!
}