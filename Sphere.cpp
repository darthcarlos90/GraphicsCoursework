#include "Sphere.h"
Mesh* Sphere::sphere = NULL;

Sphere::Sphere(Vector4 colour){
	this->colour = colour;
	//CreateSphere();
	this->mesh = sphere;
	Vector3 scale(0.0f, 0.0f, 0.0f);
	SetModelScale(scale);
	this->SetBoundingRadious(0.1f);
	grow_limit = 10;
	growth = 0.0f;
	is_rain = false;
}

Sphere::Sphere(Vector3 location, float rain_height, Vector4 color){
	this->colour = color;
	this->mesh = sphere;
	Vector3 scale(2.0f, 2.0f, 2.0f);
	SetModelScale(scale);
	this->SetBoundingRadious(0.1f);
	growth = 0.0f;
	is_rain = true;
	SetTransform(Matrix4::Translation(location));
	this->rain_height = rain_height;
}

Sphere::~Sphere(void){
	delete sphere;
}

/*
	Method: Update
		Depending on what type of figure is this, the size or
		locaton is changed.
	Params:
		msec: The milliseconds passed.
*/
void Sphere::Update(float msec){
	growth += msec;
	if(!is_rain){
		if(growth >= 1000.0f){
		
			if(grow_limit != 0){
				SetModelScale(GetModelScale() + Vector3(0.1f, 0.1f, 0.1f));
				grow_limit--;
			}
		
		growth = 0.0f;
	}
		}else {
			if(GetTransform().GetPositionVector().y > 0.0f){

				transform = transform * Matrix4::Translation(Vector3(0.0f, -20.0f, 0.0f));
			} else {
				transform = transform * Matrix4::Translation(Vector3(0.0f, rain_height, 0.0f));
			}
		}
	SceneNode::Update(msec);
}