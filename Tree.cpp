#include "Tree.h"
#include "Branch.h"

Mesh* Tree::cylinder = NULL;
Mesh* Tree::sphere = NULL;
const Vector3 TRUNK_GROWTH_RATE(0.001f, 0.001f, 0.001f); //how will the tree grow
const Vector3 BRANCH_GROWTH_RATE(0, 0.01f, 0); //how much will each branch grow
//the scaling of the shapes
const float SCALE_FACTOR_X = 0.6f;
const float SCALE_FACTOR_Y = 0.1f;
const float SCALE_FACTOR_Z = 0.6f;



Tree::Tree(int level, Vector3 location, float branching_level){
	growth = 0.0f;
	sons = 4;
	height = 70.0f;
	branchColour = Vector4(0.15f, 0.25f, 0.00001f, 1.0f);
	this->branching_level = branching_level;
	
	diameter_x = diameter_z = 8.0f;
	current_level = level;
	SetTransform(Matrix4::Translation(location));
	
	/*
		The colours used in the cylinders and spheres below are used only for testing purposes and to identify each root,
		they don't have any other importance than that.
	*/
	//create the trunk
	trunk = new SceneNode(cylinder, Vector4(1, 0, 0, 1));
	texture = SOIL_load_OGL_texture("Textures/tree_tex.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	trunk->GetMesh()->SetTexture(texture);
	glBindTexture(GL_TEXTURE_2D, trunk->GetMesh()->GetTexture());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);
	trunk->SetBoundingRadious(height);
	
	if(!trunk->GetMesh()->GetTexture())
		return;
	
	AddChild(trunk);

	// add the first branch
	branch1 = new SceneNode(cylinder, branchColour);
	branch1->SetModelScale(Vector3(SCALE_FACTOR_X,SCALE_FACTOR_Y, SCALE_FACTOR_Z));
	float branch1_height = (float)(rand())/(float)(RAND_MAX) * height;
	branch1->SetTransform(Matrix4::Translation(Vector3(0, branch1_height, diameter_z))* Matrix4::Rotation(42, Vector3(1,0,0)));
	branch1->SetBoundingRadious(branch1_height);
	trunk->AddChild(branch1);
	Branch* b = new Branch(current_level, current_level - 1,81.0f, 1, branchColour, sons, texture, branching_level);
	branch1->AddChild(b);
	Branch* b11 = new Branch(current_level, current_level - 1,81.0f, 3, branchColour, sons, texture, branching_level);
	branch1->AddChild(b11);
	

	//add a bush to the branch
	parentBush1 = new SceneNode(sphere, branchColour);
	parentBush1->SetTransform(Matrix4::Translation(Vector3(0, 10, -7)));
	parentBush1->SetModelScale(Vector3(10, 10, 10));
	parentBush1->SetBoundingRadious(10.0f);
	branch1->AddChild(parentBush1);

	//add a secondary bush to the bush
	SceneNode* bush1 = new SceneNode(sphere, branchColour);
	bush1->SetTransform(Matrix4::Translation(Vector3(0,0.5f, -10)));	
	bush1->SetModelScale(Vector3(10, 10, 10));
	bush1->SetBoundingRadious(10.0f);
	parentBush1->AddChild(bush1);
	
	//repeat the above process
	branch2 = new SceneNode(cylinder, branchColour);
	branch2-> SetModelScale(Vector3(SCALE_FACTOR_X, SCALE_FACTOR_Y, SCALE_FACTOR_Z));
	float branch2_height = (float)(rand())/(float)(RAND_MAX) * height;
	branch2->SetTransform(Matrix4::Translation(Vector3(0, branch2_height, -diameter_z)) * Matrix4::Rotation(-42, Vector3(1, 0, 0)));
	branch2->SetBoundingRadious(branch2_height);
	trunk->AddChild(branch2);
	Branch* b2 = new Branch(current_level, current_level - 1,81.0f, 4, branchColour, sons, texture, branching_level);
	branch2->AddChild(b2);
	Branch* b21 = new Branch(current_level, current_level - 1,81.0f, 3, branchColour, sons, texture, branching_level);
	branch2->AddChild(b21);

	parentBush2 = new SceneNode(sphere, branchColour);
	parentBush2->SetTransform(Matrix4::Translation(Vector3(0, 30, -7)));
	parentBush2->SetModelScale(Vector3(10, 10, 10));
	parentBush2->SetBoundingRadious(10.0f);
	branch2->AddChild(parentBush2);

	SceneNode* bush2 = new SceneNode(sphere, branchColour);
	bush2->SetTransform(Matrix4::Translation(Vector3(0,0, 10)));
	bush2->SetModelScale(Vector3(10,10,10));
	bush2->SetBoundingRadious(10.0f);
	parentBush2->AddChild(bush2);

	branch3 = new SceneNode(cylinder, branchColour);
	branch3->SetModelScale(Vector3(SCALE_FACTOR_X, SCALE_FACTOR_Y, SCALE_FACTOR_Z));
	float branch3_height = (float)(rand())/(float)(RAND_MAX) * height;	
	branch3->SetTransform(Matrix4::Translation(Vector3(diameter_x, branch3_height, 0)) * Matrix4::Rotation(-42, Vector3(0,0,1)));
	branch3->SetBoundingRadious(branch3_height);
	trunk->AddChild(branch3);
	Branch* b3 = new Branch(current_level, current_level - 1,81.0f, 4, branchColour, sons, texture, branching_level);
	branch3->AddChild(b3);
	Branch* b31 = new Branch(current_level, current_level - 1,81.0f, 1, branchColour, sons, texture, branching_level);
	branch3->AddChild(b31);

	parentBush3 = new SceneNode(sphere, branchColour);
	parentBush3->SetTransform(Matrix4::Translation(Vector3(0, -10, -7)));
	parentBush3->SetModelScale(Vector3(10, 10, 10));
	parentBush3->SetBoundingRadious(10.0f);
	branch3->AddChild(parentBush3);
	
	SceneNode* bush3 = new SceneNode(sphere, branchColour);
	bush3->SetTransform(Matrix4::Translation(Vector3(0,0, 10)));
	bush3->SetModelScale(Vector3(10,10,10));
	bush3->SetBoundingRadious(10.0f);
	parentBush3->AddChild(bush3);

	Branch* b4 = new Branch(current_level, current_level - 1,81.0f, 3, branchColour, sons + 1, texture, branching_level);
	trunk->AddChild(b4);
}



void Tree::Update(float msec){
	//The growth variable saves how many seconds have passed
	growth+= msec;

	//if 1 second has passed
	if(growth >= 100.0f){		
		//grow until you reach a limit
		if(height < 100.0f){
				//if you are the root, grow 3 small branches with their respective bushes
				ScaleTree(GetModelScale() + TRUNK_GROWTH_RATE);
				transform = transform * Matrix4::Scale(GetModelScale()) * Matrix4::Translation(Vector3(0.0f, TRUNK_GROWTH_RATE.y*500.0f, 0.0f));
				if(height* SCALE_FACTOR_Y < SCALE_FACTOR_Y * 110){
					branch1->SetModelScale(branch1->GetModelScale() + BRANCH_GROWTH_RATE);
					branch1->SetTransform(branch1->GetTransform() * Matrix4::Translation(Vector3(0, 0, BRANCH_GROWTH_RATE.y*50.0f)));
					parentBush1->SetTransform(parentBush1->GetTransform() * Matrix4::Translation(Vector3(0, BRANCH_GROWTH_RATE.y*75.0f, 0)));
				}
				if(height* SCALE_FACTOR_Y < SCALE_FACTOR_Y * 110){
					branch2->SetModelScale(branch2->GetModelScale() + BRANCH_GROWTH_RATE);
					branch2->SetTransform(branch2->GetTransform() * Matrix4::Translation(Vector3(0, 0, -BRANCH_GROWTH_RATE.y*50.0f)));
				}
				if(height* SCALE_FACTOR_Y < SCALE_FACTOR_Y * 110){
					branch3->SetModelScale(branch3->GetModelScale() + BRANCH_GROWTH_RATE);
					branch3->SetTransform(branch3->GetTransform() * Matrix4::Translation(Vector3(BRANCH_GROWTH_RATE.y*50.0f, 0, 0)));
					parentBush3->SetTransform(parentBush1->GetTransform() * Matrix4::Translation(Vector3(0, BRANCH_GROWTH_RATE.y*75.0f, 0)));
				}	
			
		} 
		//when you are finished growing, grow childs
		else {
		
			if(sons > 0){
				Branch* b = new Branch(current_level, current_level - 1, height, sons, branchColour, current_level, texture, branching_level);
				trunk->AddChild(b);
				sons--;
			}
		}
		//if a second has passed, reset the buffer
		growth = 0.0f;
	}

	//finish updating
	SceneNode::Update(msec);
}

/*
	Method: Scale Tree
		Method that scales the tree to a value.
	Params:
		value: the value to scale the tree to.
*/
void Tree::ScaleTree(Vector3 value){
	SetModelScale(value);
	height += value.y;
	diameter_x += value.x;
	diameter_z += value.z;
}