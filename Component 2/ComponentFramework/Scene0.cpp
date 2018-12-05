#include <GL/glew.h>
#include <SDL.h>
#include "Scene0.h"
#include <iostream>
#include "MMath.h"
#include "QuadSphere.h"
#include "Primitive.h"
#include "Trackball.h"

using namespace GAME;
using namespace MATH;

const float drawDistance = 100.0f;
const float fieldOfView = 45.0f;
const float nearClippingPlane = 1.0f;

const std::string skull = "Skull.obj";
const std::string horn = "Horn.obj";
const float kScaleFactor = 1.05f;
const float kMoveFactor = 0.25f;
const float kRotationFactor = 5.0f;

Scene0::Scene0(class Window& windowRef):  Scene(windowRef){ 
	trackball = new Trackball();
	sceneGraph = std::make_unique<std::map<std::string, Model *>>(std::map<std::string, Model *>());
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Scene0::~Scene0(){ 
	OnDestroy();
}

bool Scene0::OnCreate() {
	OnResize(windowPtr->GetWidth(),windowPtr->GetHeight());

	/// Load Assets: as needed 
	lightPos = Vec3(10.0f, 3.0f, 10.0f);

	sceneGraph->insert(std::make_pair(skull, new Primitive(skull.c_str())));
	sceneGraph->insert(std::make_pair(horn, new Primitive(horn.c_str())));

	return true;
}


void Scene0::OnResize(int w_, int h_){
	windowPtr->SetWindowSize(w_,h_);
	glViewport(0,0,windowPtr->GetWidth(),windowPtr->GetHeight());
	float aspect = float(windowPtr->GetWidth()) / float(windowPtr->GetHeight());
	
	projectionMatrix = MMath::perspective(fieldOfView, aspect, nearClippingPlane, drawDistance);

	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), 
							   Vec3(0.0f, 0.0f, 0.0f), 
							   Vec3(0.0f, 1.0f, 0.0f));
}

void Scene0::OnDestroy(){
	/// Cleanup Assets

	if(trackball) delete trackball;
	trackball = nullptr;
}

void Scene0::Update(const float deltaTime){

	Model* model = sceneGraph->at(skull);

	model->Update(deltaTime);
}

void Scene0::Render() const{
	/// Draw your scene here
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	/// draw code here

	for (auto& pair : *sceneGraph) {
		Model *model = pair.second;
		model->SetLightPos(viewMatrix * lightPos);
		model->Render(projectionMatrix, trackball->GetMatrix4() *  viewMatrix, trackball->GetMatrix3());
	}
	
	/// swap double buffer
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
}

void Scene0::HandleEvents(const SDL_Event& SDLEvent){
	auto triangle0 = sceneGraph->at(skull);

	if (SDLEvent.type == SDL_EventType::SDL_KEYDOWN) {
		switch (SDLEvent.key.keysym.sym) {
		case SDLK_q:
			triangle0->rotate2D(kRotationFactor);
			break;
		case SDLK_e:
			triangle0->rotate2D(-kRotationFactor);
			break;
		case SDLK_w:
			triangle0->move2D(0.0f, 1.0f * kMoveFactor);
			break;
		case SDLK_s:
			triangle0->move2D(0.0f, -1.0f * kMoveFactor);
			break;
		case SDLK_a:
			triangle0->move2D(-1.0f * kMoveFactor, 0.0f);
			break;
		case SDLK_d:
			triangle0->move2D(1.0f * kMoveFactor, 0.0f);
			break;
		case SDLK_z:
			triangle0->scale2D(kScaleFactor);
			break;
		case SDLK_x:
			triangle0->scale2D(-kScaleFactor);
			break;
		}
	}
}