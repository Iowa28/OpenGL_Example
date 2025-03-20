#include "libs.h"

Vertex vertices[] =
{
	{vec3(-.5f, .5f, 0.f), vec3(1.f, 0.f, 0.f), vec2(0.f, 1.f)},
	{vec3(-.5f, -.5f, 0.f), vec3(0.f, 1.f, 0.f), vec2(0.f, 0.f)},
	{vec3(0.5f, -.5f, 0.f), vec3(0.f, 0.f, 1.f), vec2(1.f, 0.f)},
	{vec3(.5f, .5f, 0.f), vec3(1.f, 1.f, 0.f), vec2(1.f, 1.f)}
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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

bool loadShader(GLuint& shader, const char* filename)
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

	shader = glCreateShader(GL_VERTEX_SHADER);
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
	if (!loadShader(vertexShader, "vertex_core.glsl"))
	{
		return false;
	}
	if (!loadShader(fragmentShader, "fragment_core.glsl"))
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

int main()
{
#pragma region InitOpenGL
	glfwInit();

	constexpr int WINDOW_WIDTH = 640;
	constexpr int WINDOW_HEIGHT = 480;
	// int framebufferWidth = 0;
	// int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "SHAURMA", nullptr, nullptr);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
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
	GLuint coreProgram;
	if (!loadShaders(coreProgram))
	{
		glfwTerminate();
	}
	std::cout << "load succeed" << std::endl;

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

	glBindVertexArray(0);
#pragma endregion ConfigureShaders

	GLuint texture1, texture2;
	loadTexture(texture1, "Textures/spongebob.png");
	loadTexture(texture2, "Textures/wall.jpg");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(coreProgram);

		glUniform1i(glGetUniformLocation(coreProgram, "texture1"), 0);
		glUniform1i(glGetUniformLocation(coreProgram, "texture2"), 1);
		
		glBindTextureUnit(0, texture1);
		glBindTextureUnit(1, texture2);
		
		glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	glDeleteProgram(coreProgram);

	return 0;
}