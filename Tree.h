/*
	Class: Tree
		This class represents the tree and the default branches created for that tree.
		Inherits from scenenode.
*/

#pragma once
#include "../nclgl_coursework/OBJMesh.h"
#include "../nclgl_coursework/SceneNode.h"

class Tree : public SceneNode{
public:
	/*
		Constructor.
		Params:
			level: The number of leves the tree will have.
			location: Where will the tree be position.
			branching_level: explained on the branch class.
	*/
	Tree( int level, Vector3 location, float branching_level = 1.0f);
	~Tree(void){};

	virtual void Update(float msec); 

	static void CreateCylinder(){
		OBJMesh *m = new OBJMesh();
		m->LoadOBJMesh("Meshes/cylinder_test_2.obj");
		cylinder = m;
		
	}

	static void CreateSphere(){
		OBJMesh *m = new OBJMesh();
		m->LoadOBJMesh("Meshes/sphere.obj");
		sphere = m;
	}

	static void DeleteCylinder(){ delete cylinder; }
	static void DeleteSphere(){ delete sphere; }	

private:
	int current_level; //The level of this tree
	static Mesh* cylinder;
	static Mesh* sphere;
	
	SceneNode* branch1;
	SceneNode* branch2;
	SceneNode* branch3;
	
	SceneNode* parentBush1;
	SceneNode* parentBush2;
	SceneNode* parentBush3;

	SceneNode* trunk;
	
	float growth;
	float height;
	
	//in case the base is not a complete circle
	float diameter_x;
	float diameter_z;
	float branching_level;
	
	int sons;

	Vector4 branchColour;

	void ScaleTree(Vector3 value);
	GLuint texture;
};