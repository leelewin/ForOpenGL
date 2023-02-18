#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCood;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texCood;
out vec3 normal;

void main()
{
	gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	// gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texCood = aTexCood;
	normal = aNormal;
}


