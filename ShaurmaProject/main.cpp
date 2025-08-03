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

/*
bool loadShader(GLuint& shader, const char* filename, const bool isVertexShader)
{
	char infoLog[bufferSize];
	GLint success;

	std::string temp;
	std::string src;

	std::ifstream in_file;

	in_file.open(filename);

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
		{
			src += temp + "\n";
		}
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << std::endl;
		return false;
	}

	in_file.close();

	shader = glCreateShader(isVertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(shader, 1, &vertSrc, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, bufferSize, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << std::endl;
		std::cout << infoLog << std::endl;
		return false;
	}

	return true;
}

bool loadShaders(GLuint &program)
{
	GLuint vertexShader, fragmentShader;
	if (!loadShader(vertexShader, "Shaders/vertex_core.glsl", true))
	{
		return false;
	}
	if (!loadShader(fragmentShader, "Shaders/fragment_core.glsl", false))
	{
		return false;
	}

	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint success;
	char infoLog[bufferSize];
	
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, bufferSize, nullptr, infoLog);
		std::cout << "ERROR::LOAD_SHADERS::COULD_NOT_LINK_PROGRAM" << std::endl;
		std::cout << infoLog << std::endl;
		return false;
	}

	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}
*/

void loadTexture(GLuint& texture, const char* path)
{
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image(path, &image_width, &image_height, nullptr, SOIL_LOAD_RGBA);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::LOADING_FAILED" << std::endl;	
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
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

	Shader shader = Shader("Shaders/vertex_core.glsl", "Shaders/fragment_core.glsl");
	
	// GLuint coreProgram;
	// if (!loadShaders(coreProgram))
	// {
	// 	glfwTerminate();
	// }
	// std::cout << "load succeed" << std::endl;

#pragma region ConfigureShaders
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
	
	GLuint texture1, texture2;
	loadTexture(texture1, "Textures/pusheen.png");
	loadTexture(texture2, "Textures/wall.jpg");

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

	shader.use();
	
	shader.setMat4fv(ModelMatrix, "ModelMatrix");
	shader.setMat4fv(ViewMatrix, "ViewMatrix");
	shader.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

	// glUseProgram(coreProgram);
	
	// glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMatrix"), 1, GL_FALSE, value_ptr(ModelMatrix));
	// glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ViewMatrix"), 1, GL_FALSE, value_ptr(ViewMatrix));
	// glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMatrix"), 1, GL_FALSE, value_ptr(ProjectionMatrix));
	// glUniform3fv(glGetUniformLocation(coreProgram, "lightPos0"), 1, value_ptr(lightPos0));
	// glUniform3fv(glGetUniformLocation(coreProgram, "cameraPos"), 1, value_ptr(camPosition));
	
	shader.setVec3f(lightPos0, "lightPos0");
	shader.setVec3f(camPosition, "cameraPos");
	
	// glUseProgram(0);
	shader.unuse();

#pragma endregion InitMatrices

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		updateInput(window, position, rotation, scale);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// glUseProgram(coreProgram);
		shader.use();

		// glUniform1i(glGetUniformLocation(coreProgram, "texture1"), 0);
		// glUniform1i(glGetUniformLocation(coreProgram, "texture2"), 1);
		shader.set1i(0, "texture1");
		shader.set1i(1, "texture2");

		ModelMatrix = mat4(1.f);
		ModelMatrix = translate(ModelMatrix, position);
		ModelMatrix = rotate(ModelMatrix, radians(rotation.x), vec3(1.f, 0.f, 0.f));
		ModelMatrix = rotate(ModelMatrix, radians(rotation.y), vec3(0.f, 1.f, 0.f));
		ModelMatrix = rotate(ModelMatrix, radians(rotation.z), vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, scale);
		
		// glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ModelMatrix"), 1, GL_FALSE, value_ptr(ModelMatrix));
		shader.setMat4fv(ModelMatrix, "ModelMatrix");

		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		ProjectionMatrix = perspective(radians(fov), static_cast<float>(framebufferWidth)/ framebufferHeight, nearPlane, farPlane);
		// glUniformMatrix4fv(glGetUniformLocation(coreProgram, "ProjectionMatrix"), 1, GL_FALSE, value_ptr(ProjectionMatrix));
		shader.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		glBindTextureUnit(0, texture1);
		glBindTextureUnit(1, texture2);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);

		glBindVertexArray(0);
		// glUseProgram(0);
		shader.unuse();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	// glDeleteProgram(coreProgram);

	return 0;
}