#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "texture.h"
#include "vertex_array.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

bool firstMouse = true;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
{

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	unsigned int indices[] = {


	};

	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,  0.0f,  0.0f), 
		glm::vec3( 2.0f,  5.0f, -15.0f), 
		glm::vec3(-1.5f, -2.2f, -2.5f),  
		glm::vec3(-3.8f, -2.0f, -12.3f),  
		glm::vec3( 2.4f, -0.4f, -3.5f),  
		glm::vec3(-1.7f,  3.0f, -7.5f),  
		glm::vec3( 1.3f, -2.0f, -2.5f),  
		glm::vec3( 1.5f,  2.0f, -2.5f), 
		glm::vec3( 1.5f,  0.2f, -1.5f), 
		glm::vec3(-1.3f,  1.0f, -1.5f)  
	};

	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout layout0;
	layout0.push<float>(3);
	layout0.push<float>(2);
	va.addBuffer(vb, layout0);
	va.unBind();

	//shader
	ShaderProgram shaderProgram("/home/lewin/GitHub项目本地存储库/opengl/src/vertice_shader.glsl", "/home/lewin/GitHub项目本地存储库/opengl/src/frag_shader.glsl");

	std::cout << glGetError() << std::endl;

	//texture
	std::string picture_path = "/home/lewin/GitHub项目本地存储库/opengl/texture/bookrack.jpeg";
	Texture tex1(picture_path, "texture_diffuse");

	picture_path = "/home/lewin/GitHub项目本地存储库/opengl/texture/awesomeface.png";
	Texture tex2(picture_path, "texture_diffuse");

	shaderProgram.bind();
	shaderProgram.setUniform1i("textureMapp1", 0);
	shaderProgram.setUniform1i("textureMapp2", 1);


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

		tex1.bind(0);
		tex2.bind(1);

		va.bind();

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraFront + cameraPos, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 project = glm::mat4(1.0f);
		project = glm::perspective(glm::radians(50.0f), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
		shaderProgram.setUniformMat4fv("view", glm::value_ptr(view));
		shaderProgram.setUniformMat4fv("proj", glm::value_ptr(project));

		for (unsigned int i = 0; i < 10; ++i) {
			//MVP
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(45.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));


			shaderProgram.setUniformMat4fv("model", glm::value_ptr(model));
		
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

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
