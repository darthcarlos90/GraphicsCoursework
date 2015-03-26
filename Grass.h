/*
	Class: Grass
		This class is used to represent the grass randomly growing on the field.
		This class inherits from the SceneNode class.
*/

#pragma once
#include "../nclgl_coursework/SceneNode.h"
#include "../nclgl_coursework/OBJMesh.h"

class Grass : public SceneNode{
public:
	/*
		Constructor.
		Params:
			position: The position where this grass will be placed.
	*/
	Grass(Vector3 position);

	/* Destructor. */
	~Grass(void);

	/*
		Same as the other figure generated methods, taken from Tutorial 6 code.
	*/
	static void CreateGrass(){
		OBJMesh *m = new OBJMesh();
		m->LoadOBJMesh("Meshes/grass.obj");
		cone = m;
	}
	/* Same as the other deleter */
	static void DeleteGrass() { delete cone; }

	/* Same update*/
	virtual void Update(float msec);
	

private:
	//The figure used to draw grass
	static Mesh* cone;

	//true if the grass finished "growing"
	bool grown;

	//True if the grass finished bending
	bool bended;

	//The limit the grass will grow
	float height_limit;

	//the actual height of the grass
	float height;

	//the inclination of this grass
	float inclination;

	//The leafs of the grass
	SceneNode* leaf1;
	SceneNode* leaf2;
	SceneNode* leaf3;
};