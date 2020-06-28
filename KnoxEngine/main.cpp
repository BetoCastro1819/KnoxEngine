#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>


const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
"}\n";

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
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

int main()
{
	glfwInit();
	
	// Setup to target the OpenGL version 3.3 core-profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Knox Engine", NULL, NULL);
	if (!window)
	{
		printf("Failed to create GLFW windows\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// Vertex Shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR: Failed to compile VertexShader!\n");
		printf("INFO_LOG: %s\n", infoLog);
	}
	else
	{
		printf("Vertex shader compiled successfully\n");
	}

	// Fragment Shader
	unsigned int fragmentShader;
	fragmentShader= glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR: Failed to compile FragmentShader!\n");
		printf("INFO_LOG: %s\n", infoLog);
	}
	else
	{
		printf("Fragment shader compiled successfully\n");
	}

	// Shader Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR: Failed to link ShaderProgram!\n");
		printf("INFO_LOG: %s\n", infoLog);
	}
	else
	{
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		printf("ShaderProgram linked successfully\n");
	}
	
	// Vertex (and buffers) setup and configuration
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f
	};

	unsigned int vertexArrayObj, vertexBufferObj;
	glGenVertexArrays(1, &vertexArrayObj);
	glGenBuffers(1, &vertexBufferObj);

	glBindVertexArray(vertexArrayObj);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObj);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	// Unbinding vertexBufferObj, the call to glVertexAttribPointer register the vertexBufferObj as 
	// the vertex attribute's bound vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// We also unbind the vertexArrayObject so other calls won't accidentally modify it
	glBindVertexArray(0);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vertexArrayObj);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vertexArrayObj);
	glDeleteBuffers(1, &vertexBufferObj);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}