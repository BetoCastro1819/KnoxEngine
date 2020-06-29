#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 pos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"}\n\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0);\n"
"}\n\0";

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
bool createShader(GLenum shaderType, unsigned int &shader, const char* shaderSource);
bool createShaderProgram(unsigned int &shaderProgram, unsigned int vertexShader, unsigned int fragmentShader);

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

	unsigned int vertexShader;
	if (!createShader(GL_VERTEX_SHADER, vertexShader, vertexShaderSource))
	{
		return -1;
	}

	unsigned int fragmentShader;
	if (!createShader(GL_FRAGMENT_SHADER, fragmentShader, fragmentShaderSource))
	{
		return -1;
	}

	// Shader Program
	unsigned int shaderProgram;
	if (!createShaderProgram(shaderProgram, vertexShader, fragmentShader))
	{
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	// Vertex (and buffers) setup and configuration
	float triangle_1[] = {
		-0.75f, -0.25f, 0.0f,						 
		-0.5f, 0.25f, 0.0f,						 
		-0.25f, -0.25f, 0.0f, 
	};

	float triangle_2[] = {
		0.25f, -0.25f, 0.0f,
		0.5f, 0.25f, 0.0f,
		0.75f, -0.25f, 0.0f
	};

	//unsigned int indices[] = { 
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};	

	unsigned int VAO[2], VBO[2]; // EBO;
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_2), triangle_2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Unbinding VBO, the call to glVertexAttribPointer register the VBO as 
	// the vertex attribute's bound vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// We DON'T unbind the EBO while the VAO is active because the EBO is stored in the VAO
	//glBindVertexArray(GL_ELEMENT_ARRAY_BUFFER, 0);

	// We can unbind the VBO so other calls don't accidentally modify it
	glBindVertexArray(0);

	// Uncomment to draw wireframes
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render Loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glBindVertexArray(0); // No need to unbind it every time

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

bool createShader(GLenum shaderType, unsigned int &shader, const char* shaderSource)
{
	shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("ERROR: Failed to compile VertexShader!\n");
		printf("INFO_LOG: %s\n", infoLog);
		
		return false;
	}
	else
	{
		printf("Vertex shader compiled successfully\n");
		return true;
	}
}

bool createShaderProgram(unsigned int &shaderProgram, unsigned int vertexShader, unsigned int fragmentShader)
{
	shaderProgram = glCreateProgram();
	
	if (!vertexShader)
	{
		printf("ERROR: No vertex shader provided\n");
		return false;
	}
	if (!fragmentShader)
	{
		printf("ERROR: No fragment shader provided\n");
		return false;
	}

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("ERROR: Failed to link ShaderProgram!\n");
		printf("INFO_LOG: %s\n", infoLog);

		return false;
	}
	else
	{
		printf("ShaderProgram linked successfully\n");

		return true;
	}
}


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
