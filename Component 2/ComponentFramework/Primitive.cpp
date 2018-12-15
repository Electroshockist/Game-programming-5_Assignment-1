#include "Primitive.h"
#include "Shader.h"
#include "ObjLoader.h"
#include <iostream>
#include <cassert>

Primitive::Primitive(GLenum drawmode_, std::vector<Vec3> *vertices_, std::vector<Vec3> *normals_, std::vector<Vec2> *uvs_) {
	meshes.push_back(new Mesh(drawmode_, *vertices_, *normals_, *uvs_));
	OnCreate();
}

Primitive::Primitive(const char* filepath) {
	ObjLoader obj(filepath);
	meshes.push_back(new Mesh(GL_TRIANGLES, obj.vertices, obj.normals, obj.uvCoords));
	OnCreate();
}

Primitive::~Primitive() {

}

bool Primitive::OnCreate() {
	shader = new GAME::Shader("phongVert.glsl", "phongFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
	projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
	modelViewMatrixID = glGetUniformLocation(shader->getProgram(), "modelViewMatrix");
	normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
	lightPosID = glGetUniformLocation(shader->getProgram(), "lightPos");
	modelMatrix = center();
	translationMatrix = MMath::translate(0.0f, 0.0f, 0.0f);
	rotationMatrix = MMath::translate(0.0f, 0.0f, 0.0f);
	transformationMatrix = MMath::translate(0.0f, 0.0f, 0.0f);
	scaleMatrix = Matrix4(1.0f);
	scaleFactor = kDefaultScaleFactor;
	isDirty = true;
	return true;
}

Matrix4 Primitive::center() {
	auto first = meshes[0];
	auto temp = Vec3(0.0f, 0.0f, 0.0f); 

	for (auto vector : first->vertices) {
		temp = temp + vector;
	}

	temp = temp / (first->vertices.size() * 1.0f);
	return MMath::translate(-temp.x, -temp.y, -temp.z);
}

void Primitive::addChild(Primitive* child) {
	children.push_back(child);
	child->parent = this;

}

//void Primitive::addChild(Primitive* child) {
//	children.push_back(*child);
//}

void Primitive::OnDestroy() {
	glDeleteBuffers(1, &vbo);
	if (shader) delete shader;

	for (Mesh* mesh : meshes) {
		if (mesh) delete mesh;
	}
}

void Primitive::Update(const float deltaTime) {

	/// Code your physics below
}

void Primitive::Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Matrix3& normalMatrix) const {
	Matrix4 resultModelMatrix;
	if (parent != nullptr) {
		resultModelMatrix = static_cast<Primitive*>(parent)->modelMatrix * modelMatrix;
	}
	else resultModelMatrix = modelMatrix;

	glUseProgram(shader->getProgram());
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, viewMatrix *  resultModelMatrix * transformationMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	glUniform3fv(lightPosID, 1, lightPos);
	for (auto mesh : meshes) {
		mesh->Render();
	}
	glEnableVertexAttribArray(0);
}

void Primitive::updateTransformationMatrix2D() {
	transformationMatrix = rotationMatrix * MMath::scale(scaleFactor * 1.0f, scaleFactor * 1.0f, 1.0f) * translationMatrix;
}

void Primitive::updateTransformationMatrix3D() {
	transformationMatrix = rotationMatrix * scaleMatrix * translationMatrix;
}

void Primitive::SetLightPos(const Vec3& lightPos_) {
	lightPos = lightPos_;
}

void Primitive::rotate2D(float degrees) {
	rotationMatrix = rotationMatrix * MMath::rotate(degrees, 0.0f, 0.0f, 1.0f);
	updateTransformationMatrix2D();
}

void Primitive::rotate3D(const Vec4 axis) {
	// TODO: implement with quaternion math
}

void Primitive::move2D(const float x_, const float y_) {
	translationMatrix = translationMatrix * MMath::translate(x_, y_, 0.0f);
	updateTransformationMatrix2D();
}

void Primitive::move3D(Vec3 direction, float amount) {
	translationMatrix = translationMatrix * MMath::translate(direction.x * amount, direction.y * amount, direction.z * amount);
	updateTransformationMatrix3D();
}

void Primitive::scale2D(float amount) {
	assert(amount != 0);
	if (amount > 0) {
		scaleFactor = scaleFactor * amount;
		updateTransformationMatrix2D();
		return;
	}
	scaleFactor = scaleFactor / (amount *-1.0f);
	updateTransformationMatrix2D();
}

void Primitive::scale3D(const Vec3 axis) {
	scaleMatrix = MMath::scale(axis.x, axis.y, axis.z);
}