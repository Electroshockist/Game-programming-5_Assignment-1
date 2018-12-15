#pragma once
#include "Model.h"
#include <memory>
#include "Shader.h"
#include <vector>

using namespace GAME;

class Primitive :public Model {
public:
	Primitive(GLenum drawmode_, std::vector<Vec3> *vertices_, std::vector<Vec3> *normals_, std::vector<Vec2> *uvs_);
	Primitive(const char* filePath);

	std::vector<Model*> children;

	Primitive();
	Primitive(const Primitive&) = delete;
	Primitive(Primitive&&) = delete;
	Primitive& operator = (const Primitive&) = delete;
	Primitive& operator = (Primitive&&) = delete;

	virtual bool OnCreate();
	virtual void OnDestroy();
	virtual void Update(const float deltaTime);
	virtual void Render(const Matrix4& projectionMatrix, const Matrix4& modelViewMatrix, const Matrix3& normalMatrix) const;
	virtual ~Primitive();

	void SetLightPos(const Vec3&);
	void rotate2D(const float degrees);
	void rotate3D(const Vec4 quat);
	void move2D(const float x_, const float y_);
	void move3D(const Vec3 direction, const float amount);
	void scale2D(const float amount);
	void scale3D(const Vec3 axis);
	void addChild(Primitive* child);

private:
	GAME::Shader *shader;
	GLuint vbo;
	GLuint vao;
	GLint verticiesID;
	GLint normalsID;
	GLint uvCoordsID;

	GLint projectionMatrixID;
	GLint modelViewMatrixID;
	GLint normalMatrixID;
	GLint lightPosID;
	Vec3 lightPos;

	Matrix4 rotationMatrix;
	Matrix4 translationMatrix;
	Matrix4 scaleMatrix;
	Matrix4 transformationMatrix;
	
	float scaleFactor;
	bool isDirty = true;

	Matrix4 center();
	void updateTransformationMatrix2D();
	void updateTransformationMatrix3D();
};