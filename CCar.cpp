#include "CCar.h"
#include "CWorld.h"
#include "CCamera.h"

CCar::CCar()
	: m_model("objects/cat_car.obj"), Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	Position = glm::vec3(10.0f, 0.2f, 0.0f);
	Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

CCar::~CCar()
{
}

void CCar::Render(Shader shader, CWorld &world, float delta)
{
	m_velocity = glm::clamp(m_velocity, -2.0f, 2.0f);

	// The exponential function is pretty good until the lower speeds
	// I could probably adjust the y offset to make it better, but instead we
	// will just use a linear function x*0.9 for now
	if (m_velocity > 0.05f || m_velocity < -0.05f)
	{
		float velocityDecayFactor = (isStunned ? 1.5f : 2.5f);
		float velocityDecay = (pow(velocityDecayFactor, abs(m_velocity)) - 0.5f) * delta;

		if (velocityDecay > abs(m_velocity))
		{
			printf("decay would be greater than velocity\n");
			m_velocity *= 0.9f;
			isStunned = false;
		}
		else
		{
			float cur_velocity = m_velocity;
			if (m_velocity > 0)
				m_velocity = m_velocity - (velocityDecay);
			else
				m_velocity = m_velocity + (velocityDecay);
		}
	}
	else
	{
		m_velocity *= 0.9f;
	}

	if (isStunned && m_velocity < 0.05 && m_velocity > -0.05)
	{
		isStunned = false;
	}

	auto newX = Position.x + m_velocity * sin(glm::radians(Rotation.y));
	auto newZ = Position.z + m_velocity * cos(glm::radians(Rotation.y));
	if (world.Collides(glm::vec3(newX, Position.y, newZ)))
	{
		m_velocity = -m_velocity * 0.5f;
		isStunned = true;
	}
	else
	{
		Position.x = newX;
		Position.z = newZ;
	}

	glm::mat4 model = glm::mat4(1.0f);
	// Translate the car to its position
	model = glm::translate(model, glm::vec3(Position.x, Position.y, Position.z));
	// Rotate the car relative to its own axis
	model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	shader.setMat4("model", model);
	m_model.Draw(shader);
}

void CCar::ProcessKeyboard(Car_Movement direction, float deltaTime)
{
	float velocity = 2.5f * deltaTime;
	if (direction == CAR_LEFT)
		Rotation.y += 90.0f * deltaTime;
	if (direction == CAR_RIGHT)
		Rotation.y -= 90.0f * deltaTime;
	if (!isStunned)
	{
		if (direction == CAR_FORWARD)
		{
			m_velocity = glm::max(m_velocity - velocity, -1.5f);
		}
		if (direction == CAR_BACKWARD)
		{
			m_velocity = glm::min(m_velocity + velocity, 0.5f);
		}
		if (direction == CAR_FORWARD_FAST)
		{
			m_velocity = glm::max(m_velocity - (velocity * 3.5f), -2.5f);
		}
	}
	updateFrontVector();
}

void CCar::updateFrontVector()
{
	Front = glm::vec3(cos(glm::radians(-Rotation.y)), 0.0f, sin(glm::radians(-Rotation.y)));
	Front = glm::normalize(Front);
}
