#include "CWorld.h"
#include "shader_s.h"

CWorld::CWorld()
: m_model("objects/world.obj")
{
}

CWorld::~CWorld()
{
}

void CWorld::Render(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(140.0f, 10.0f, 40.0f));
    shader.setMat4("model", model);
    m_model.Draw(shader);
}

bool CWorld::Collides(glm::vec3 position)
{
    bool collx = position.x > 139.0f || position.x < -139.0f;
    bool collz = position.z > 39.0f || position.z < -39.0f;
    bool collides_world = collx || collz;
    if (collides_world)
    {
        return true;
    }

    collx = position.x > -1.25 && position.x < 1.25f;
    collz = position.z > -1.25 && position.z < 1.25f;
    bool collides_sign = collx && collz;
    return collides_sign;
}
