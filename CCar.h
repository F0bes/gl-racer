#pragma once

#include "CModel.h"
#include "CWorld.h"
#include "shader_s.h"
#include "CCamera.h"

enum Car_Movement {
    CAR_FORWARD,
    CAR_BACKWARD,
    CAR_LEFT,
    CAR_RIGHT,
	CAR_FORWARD_FAST,
};

class CCar
{
public:
	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Front;

	CCar();
	~CCar();

	void Render(Shader shader, CWorld& world, float delta);
	void ProcessKeyboard(Car_Movement direction, float deltaTime);

private:
	Model m_model;
	float m_velocity = 0.0f;
	void updateFrontVector();
	bool isStunned = false;
};
