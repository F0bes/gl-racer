

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>

#include "shader_s.h"
#include "CModel.h"
#include "CCamera.h"
#include "CWorld.h"
#include "CCar.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 2.0f, 3.0f));
CCar* car = nullptr;
bool keyPressed = false;

float timeSinceLastKeyPress = 0.0f;
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(car == nullptr)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        car->ProcessKeyboard(CAR_FORWARD, deltaTime);
        camera.ProcessKeyboard(FORWARD, deltaTime);
        timeSinceLastKeyPress = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        car->ProcessKeyboard(CAR_BACKWARD, deltaTime);
        camera.ProcessKeyboard(BACKWARD, deltaTime);
        timeSinceLastKeyPress = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        car->ProcessKeyboard(CAR_LEFT, deltaTime);
        camera.ProcessKeyboard(LEFT, deltaTime);
        timeSinceLastKeyPress = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        car->ProcessKeyboard(CAR_RIGHT, deltaTime);
        camera.ProcessKeyboard(RIGHT, deltaTime);
        timeSinceLastKeyPress = glfwGetTime();
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        car->ProcessKeyboard(CAR_FORWARD_FAST, deltaTime);
        timeSinceLastKeyPress = glfwGetTime();
    }
}

bool firstMouse = true;

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int offset = 0;
int main(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    timeSinceLastKeyPress = glfwGetTime() - 1.0f;
    GLFWwindow *window = glfwCreateWindow(640, 480, "gl", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, 640, 480);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader ourShader("shaders/4.0.shader.vs", "shaders/4.0.shader.fs");

    Model ourModel("objects/sign.obj");

    CWorld world;
    car = new CCar();
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)640 / (float)480, 0.1f, 300.0f);
        glm::mat4 view;
        if((glfwGetTime() - timeSinceLastKeyPress) < 1.0f )
        {
            view = camera.GetViewMatrix(car->Position, car->Rotation);
            offset = 0;
        }
        else
        {
            offset++;
            view = camera.GetViewMatrix(car->Position, glm::vec3(car->Rotation.x, car->Rotation.y + offset, car->Rotation.z));
        }

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        world.Render(ourShader);
        car->Render(ourShader, world, deltaTime);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, car->Front);
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        model = glm::rotate(model, glm::radians((float)glfwGetTime() * 90), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
