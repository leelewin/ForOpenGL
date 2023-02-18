#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "model.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

bool firstMouse = true;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 light_pos = glm::vec3(2.0f, 2.0f, 8.0f);

float current_time = 0.0f;
float last_time = 0.0f;
float delta_time = 0.0f;

double cursor_last_xpos = 800.0f / 2;
double cursor_last_ypos = 600.0f / 2;

float yaw = -90.0f;
float pitch = 0.0f;

void processInput(GLFWwindow* window);
void mouseCallBack(GLFWwindow* window, double x_pos, double y_pos);

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//for debug
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpengl", NULL, NULL);
	if (window == NULL) {
		std::cout << "fail to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseCallBack);

	//隐藏光标并捕获
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "fail to init GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//for debug
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "启用调试上下文成功" << std::endl;
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

{
	std::string model_path = "/home/lewin/GitHub项目本地存储库/opengl/model/nanosuit/nanosuit.obj";

	Model our_model(model_path.c_str());


	//shader
	ShaderProgram shaderProgram("/home/lewin/GitHub项目本地存储库/opengl/src/vertice_shader.glsl", "/home/lewin/GitHub项目本地存储库/opengl/src/frag_shader.glsl");

	std::cout << glGetError() << std::endl;

	glEnable(GL_DEPTH_TEST);
	//渲染循环
	while (!glfwWindowShouldClose(window)) {

		current_time = glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;

		//input
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 0.2f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.bind();
		//view/projection
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraFront + cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 project = glm::mat4(1.0f);
		project = glm::perspective(glm::radians(50.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		shaderProgram.setUniformMat4fv("view", glm::value_ptr(view));
		shaderProgram.setUniformMat4fv("proj", glm::value_ptr(project));
		shaderProgram.setUniform3f("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
		shaderProgram.setUniform3f("light.position", light_pos.x, light_pos.y, light_pos.z);
		shaderProgram.setUniform3f("light.ambient", 0.2, 0.2, 0.2);
		shaderProgram.setUniform3f("light.diffuse", 0.5, 0.5, 0.5);
		shaderProgram.setUniform3f("light.specular", 0.6, 0.6, 0.6);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		// model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		shaderProgram.setUniformMat4fv("model", glm::value_ptr(model));
	
		//render
		our_model.draw(shaderProgram);
	

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

}
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 1.0f * delta_time;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraFront));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraUp, cameraFront));

}

void mouseCallBack(GLFWwindow* window, double x_pos, double y_pos)
{
	if (firstMouse) {
		cursor_last_xpos = x_pos;
		cursor_last_ypos = y_pos;
		firstMouse = false;
	}
	//屏幕原点在左上角
	float offset_x = static_cast<float>(x_pos - cursor_last_xpos);
	float offset_y = static_cast<float>(cursor_last_ypos - y_pos);
	cursor_last_xpos = x_pos;
	cursor_last_ypos = y_pos;

	float sensitivity = 0.05f;
	offset_x *= sensitivity;
	offset_y *= sensitivity;

	yaw += offset_x;
	pitch += offset_y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
	

}
