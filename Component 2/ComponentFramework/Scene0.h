#ifndef Scene0_H
#define Scene0_H

#include "Scene.h"
#include "Window.h"
#include <map>
#include <memory>

namespace GAME {
	/// Forward casting
	class Trackball;
	class Model;

	class Scene0 : public Scene  {
	protected:
		
	public:
		explicit Scene0(Window& windowRef);
		virtual ~Scene0();

		/// Delete these possible default constructors and operators  
		Scene0(const Scene0&) = delete;
		Scene0(Scene0 &&) = delete;
		Scene0& operator=(const Scene0 &) = delete;
		Scene0& operator=(Scene0 &&) = delete;

		virtual bool OnCreate() ;
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render() const;
		virtual void HandleEvents(const SDL_Event &SDLEvent);
		virtual void OnResize(const int, const int);
	
	private:
		Vec3 lightPos;
		std::unique_ptr<std::map<std::string, Model *>> sceneGraph;
		Trackball *trackball;
	};
}
#endif