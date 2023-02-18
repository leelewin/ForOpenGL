
add_executable(renderer main.cc glad.c 
	vertexBuffer.cc 
	indexBuffer.cc
	shader.cc
	texture.cc
	vertexArray.cc
	mesh.cc
	model.cc
	debug.cc

	
)

target_link_libraries(renderer
	-lglfw
	-lGL
	-lm
	-lXrandr
	-lXi
	-lX11
	-lXxf86vm
	-lpthread
	-ldl
	-lXinerama
	-lXcursor
	-lassimp

)