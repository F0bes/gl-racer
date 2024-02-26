#pragma once
#include "CModel.h"
#include "shader_s.h"

class CWorld
{
public:
	CWorld();
	~CWorld();

	void Init();
	void Update();
	void Render(Shader shader);
	void Release();
	bool Collides(glm::vec3 position);

private:
	Model m_model;
};
