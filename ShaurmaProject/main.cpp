#include "libs.h"

// Vertex vertices[] =
// {
// 	{vec3(-.5f, .5f, 0.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f)},
// 	{vec3(-.5f, -.5f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f)},
// 	{vec3(0.5f, -.5f, 0.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f)},
// 	{vec3(.5f, .5f, 0.f), vec3(1.f, 1.f, 0.f), vec2(1.f, 1.f)}
// };

Vertex vertices[] =
{
	{vec3(-.5f, .5f, 0.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f), vec3(0.f, 0.f, -1.f)}, 
	{vec3(-.5f, -.5f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f), vec3(0.f, 0.f, -1.f)},
	{vec3(0.5f, -.5f, 0.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f), vec3(0.f, 0.f, -1.f)},
	{vec3(.5f, .5f, 0.f), vec3(1.f, 1.f, 0.f), vec2(1.f, 1.f), vec3(0.f, 0.f, -1.f)}
};

signed nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

signed nrOfIndices = sizeof(indices) / sizeof(GLuint);

constexpr uint16_t bufferSize = 512;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void updateInput(GLFWwindow* window, vec3& position, vec3& rotation, vec3& scale)
{
	constexpr float speed = .001f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += speed;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= .1f;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += .1f;
	}
}

int main()
{
#pragma region InitOpenGL
	glfwInit();

	constexpr int WINDOW_WIDTH = 640;
	constexpr int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SHAURMA", nullptr, nullptr);

	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glViewport(0, 0, framebufferWidth, framebufferHeight);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << std::endl;
		glfwTerminate();
	}
#pragma endregion InitOpenGL

#pragma region RenderSettings
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
#pragma endregion RenderSettings

#pragma region ConfigureShaders
	Shader coreProgram = Shader("Shaders/vertex_core.glsl", "Shaders/fragment_core.glsl");
	
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
#pragma endregion ConfigureShaders

#pragma region InitMatrices
	Texture texture1 = Texture("Textures/pusheen.png", GL_TEXTURE_2D, 0);
	Texture texture2 = Texture("Textures/wall.jpg", GL_TEXTURE_2D, 1);

	Material material = Material(
		vec3(1.f),
		vec3(1.f),
		vec3(1.f),
		texture1.getTextureUnit(),
		texture2.getTextureUnit()
	);

	vec3 position = vec3(0.f);
	vec3 rotation = vec3(0.f);
	vec3 scale = vec3(1.f);

	mat4 ModelMatrix = mat4(1.f);
	ModelMatrix = translate(ModelMatrix, position);
	ModelMatrix = rotate(ModelMatrix, radians(rotation.x), vec3(1.f, 0.f, 0.f));
	ModelMatrix = rotate(ModelMatrix, radians(rotation.y), vec3(0.f, 1.f, 0.f));
	ModelMatrix = rotate(ModelMatrix, radians(rotation.z), vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, scale);

	vec3 camPosition = vec3(0.f, 0.f, 1.f);
	vec3 worldUp = vec3(0.f, 1.f, 0.f);
	vec3 camFront = vec3(0.f, 0.f, -1.f);
	
	mat4 ViewMatrix = mat4(1.f);
	ViewMatrix = lookAt(camPosition, camPosition + camFront, worldUp);

	float fov = 90.f;
	float nearPlane = .1f;
	float farPlane = 1000.f;
	mat4 ProjectionMatrix = mat4(1.f);
	ProjectionMatrix = perspective(radians(fov), static_cast<float>(framebufferWidth)/ framebufferHeight, nearPlane, farPlane);

	vec3 lightPos0 = vec3(0.f, 0.f, 1.f);

	coreProgram.use();
	
	coreProgram.setMat4fv(ModelMatrix, "ModelMatrix");
	coreProgram.setMat4fv(ViewMatrix, "ViewMatrix");
	coreProgram.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	
	coreProgram.setVec3f(lightPos0, "lightPos0");
	coreProgram.setVec3f(camPosition, "cameraPos");
	
	coreProgram.unuse();

#pragma endregion InitMatrices

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		updateInput(window, position, rotation, scale);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		coreProgram.use();

		material.sendToShader(coreProgram);

		ModelMatrix = mat4(1.f);
		ModelMatrix = translate(ModelMatrix, position);
		ModelMatrix = rotate(ModelMatrix, radians(rotation.x), vec3(1.f, 0.f, 0.f));
		ModelMatrix = rotate(ModelMatrix, radians(rotation.y), vec3(0.f, 1.f, 0.f));
		ModelMatrix = rotate(ModelMatrix, radians(rotation.z), vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, scale);
		
		coreProgram.setMat4fv(ModelMatrix, "ModelMatrix");

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = perspective(radians(fov), static_cast<float>(framebufferWidth)/ framebufferHeight, nearPlane, farPlane);

		coreProgram.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		texture1.bind();
		texture2.bind();
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);

		glBindVertexArray(0);

		coreProgram.unuse();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}