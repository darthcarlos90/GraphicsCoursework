#include <iostream>
#include "../nclgl_coursework/Window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")


int main(void){
	Window w("Coursework", 1000, 800, false);

	if(!w.HasInitialised()){
		std::cout << "The window couldn't be loaded!" << endl;
		std::cout << "Press a number to exit" << endl;
		int exit;
		cin >> exit;
		return -1;
	}


	Renderer* renderer = new Renderer(w);
	if(!renderer->HasInitialised()) {
		return -1;
	}
	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if(Window::GetKeyboard()->KeyDown(KEYBOARD_5)){
			renderer = NULL;
			renderer = new Renderer(w);
		} 
		
		renderer->UpdateScene(w.GetTimer()->GetTimedMS());
		renderer->RenderScene();		
	}

	delete renderer;
	
	return 0;

}