#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "texture.h"
#include "vertexArray.h"
#include "mesh.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
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
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, //0
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  //1
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //2
		0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  //2
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,0.0f, 1.0f, //3
		// -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //0

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f, //4
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f, 0.0f,  //5
		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f, 1.0f,  //6
		// 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  //6
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f, 1.0f, //7
		// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //4

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f, 0.0f, //8
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,1.0f, 1.0f, //9
		// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //0
		// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //0
		// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //4
		// -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, //8

		0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f, 0.0f,  //10
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //2
		// 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  //11
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,0.0f, 1.0f,  //11
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f,  //12
		// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //10

		// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //0
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,1.0f, 1.0f,  //13
		// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  //5
		// 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  //5
		// -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //4
		// -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, //0

		// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //3
		// 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  //2
		// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //10
		// 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  //10
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,0.0f, 0.0f //14
		// -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  //3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
		4, 5, 6,
		6, 7, 4,
		8, 9, 0,
		0, 4, 8,
	   10, 2,11,
	   11,12,10,
	    0, 13, 5,
		5, 4, 0,
		3, 2, 10,
		10, 14, 3
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

	std::vector<Vertex> vertices_s;
	for (int i = 0; i < sizeof(vertices) / 4; i += 8) {
		Vertex v;
		v.pos.x = vertices[i];
		v.pos.y = vertices[i+1];
		v.pos.z = vertices[i+2];
		v.normal.x = vertices[i+3];
		v.normal.y = vertices[i+4];
		v.normal.z = vertices[i+5];
		v.tex_coords.x = vertices[i+6];
		v.tex_coords.y = vertices[i+7];
		vertices_s.push_back(v);
	}

	std::cout << vertices_s.size() << std::endl;

	std::vector<unsigned int> indices_s;
	for (int i = 0; i < 36; ++i) {
		indices_s.push_back(indices[i]);
	}




	VertexArray va;
	VertexBuffer vb(vertices, sizeof(vertices));
	IndexBuffer ib(indices, 36);
	VertexBufferLayout layout0;
	layout0.push<float>(3);
	layout0.push<float>(3);
	layout0.push<float>(2);
	va.addBuffer(vb, layout0);
	va.addIndices(ib);
	va.unBind();

	//shader
	ShaderProgram shaderProgram("/home/lewin/GitHub项目本地存储库/opengl/src/vertice_shader.glsl", "/home/lewin/GitHub项目本地存储库/opengl/src/frag_shader.glsl");

	std::cout << glGetError() << std::endl;

	//texture
	std::string picture_path = "/home/lewin/GitHub项目本地存储库/opengl/texture/bookrack.jpeg";
	Texture tex1(picture_path, "texture_diffuse");

	picture_path = "/home/lewin/GitHub项目本地存储库/opengl/texture/awesomeface.png";
	Texture tex2(picture_path, "texture_specular");

	// std::vector<std::shared_ptr<Texture>> textures;
	// textures.push_back(std::shared_ptr<Texture>(&tex1));
	// textures.push_back(std::shared_ptr<Texture>(&tex2));

	shaderProgram.bind();
	shaderProgram.setUniform1i("texture_diffuse1", 0);
	shaderProgram.setUniform1i("texture_specular1", 1);

	tex1.bind(0);
	tex2.bind(1);
	// Mesh mesh(vertices_s, indices_s, textures);

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


		// tex1.bind();
		// tex2.bind();

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
		
			// glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
			// mesh.draw(shaderProgram);

		}
		va.unBind();

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
