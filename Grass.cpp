#include "Grass.h"
Mesh* Grass::cone = NULL;

/*Constructor*/
Grass::Grass(Vector3 position){
	//Create the nodes, add them color and add them to this scene tree
	leaf1 = new SceneNode(cone, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	leaf1->SetModelScale(Vector3(25,25,25));
	AddChild(leaf1);
	leaf2 = new SceneNode(cone, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	leaf2->SetModelScale(Vector3(25,25,25));
	AddChild(leaf2);
	leaf3 = new SceneNode(cone, Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	leaf3->SetModelScale(Vector3(25,25,25));
	AddChild(leaf3);

	//move this entire tree to the position stated
	SetTransform(Matrix4::Translation(position));

	//set varuables
	height_limit = 30.0f;
	height = 0.0f;
	inclination = 0.0f;
	grown = false;
	bended = false;

	//set bounding radius
	this->SetBoundingRadious(height_limit);
}

Grass::~Grass(void){
	delete leaf1;
	delete leaf2;
	delete leaf3;
}

/*Update class*/
void Grass::Update(float msec){
	//First make the grass "grow" upwards.
	if(!grown){
		leaf1->SetTransform(
			leaf1->GetTransform() * 
			Matrix4::Translation(Vector3(0.0f, 0.1f, 0.0f)));
		leaf2->SetTransform(
			leaf2->GetTransform() * 
			Matrix4::Translation(Vector3(0.0f, 0.1f, 0.0f)));
		leaf3->SetTransform(
			leaf3->GetTransform() * 
			Matrix4::Translation(Vector3(0.0f, 0.1f, 0.0f)));
		height += 0.1f;
		//set the grown flag to true when it finished growing
		if(height >= height_limit)
			grown = true;
		//When it finished growing, bend it
	} else {
		if(!bended){
			leaf1->SetTransform(leaf1->GetTransform() *
				Matrix4::Rotation(-0.01f, Vector3(1,0,0)) * 
				Matrix4::Translation(Vector3(0.0f, 0.0f, -0.0055f)));
			leaf2->SetTransform(leaf2->GetTransform() * 
				Matrix4::Rotation(0.01f, Vector3(0, 0, 1))
				* Matrix4::Translation(Vector3(-0.0055f,0.0f,0.0f)));
			leaf3->SetTransform(leaf3->GetTransform() * 
				Matrix4::Rotation(-0.01f, Vector3(0,0,1))
				* Matrix4::Translation(Vector3(0.0055f,0.0f,0.0f)));
			inclination += 0.01f;
			if(inclination >= 45.0f)
				bended = true;
		}
	}
	SceneNode::Update(msec);
}