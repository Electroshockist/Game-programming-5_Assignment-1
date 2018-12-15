#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "MMath.h"
#include "Mesh.h"
#include <memory>

using namespace MATH;

class Scene;

namespace GAME {

	class Model {
	protected:
		Vec3 pos;
		Vec3 vel;
		Vec3 orientation;
		Vec3 gravity;
		Sphere bounding;
		Matrix4 modelMatrix;
		Scene* parentScene;
		std::vector<Mesh*> meshes;
	public:

		void* parent = nullptr;

		inline void SetPos(const Vec3& pos_) { pos = pos_; }
		inline void SetVel(const Vec3& vel_) { vel = vel_; }

		virtual ~Model() {};

		Model(const Model&) = delete;
		Model(Model&&) = delete;
		Model& operator = (const Model&) = delete;
		Model& operator = (Model&&) = delete;

		Model() : gravity(0.0f, -9.8f, 0.0f) { };

		virtual void SetLightPos(const Vec3&) = 0;
		virtual bool OnCreate() = 0;
		virtual void OnDestroy() = 0;
		virtual void Render(const Matrix4& projectionMatrix, const Matrix4& modelViewMatrix, const Matrix3& normalMatrix)const = 0;
		virtual void Update(const float deltaTime) = 0;

		std::vector<Mesh*>* getMeshes() {
			return &meshes;
		}

		virtual void rotate2D(const float degrees) = 0;
		virtual void rotate3D(const Vec4 quat) = 0;
		virtual void move2D(const float x_, const float y_) = 0;
		virtual void move3D(const Vec3 direction, const float amount) = 0;
		virtual void scale2D(const float amount) = 0;
		virtual void scale3D(const Vec3 axis) = 0;
	};

} /// end of namespace

#endif