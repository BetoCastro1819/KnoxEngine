#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include <iostream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	glfwInit();
	
	// Setup to target the OpenGL version 3.3 core-profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "Knox Engine", NULL, NULL);
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

	// Vertex (and buffers) setup and configuration
	float triangle_1[] = {
		// positions		 // colors
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,
	};

	//unsigned int indices[] = { 
	//	0, 1, 3, // first triangle
	//	1, 2, 3  // second triangle
	//};	

	unsigned int VAO[1], VBO[1]; // EBO;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);

	// 3 floats for position + 3 floats for colors
	GLsizei vertexStride = sizeof(float) * 6; 
	
	// Vertex attribute al (location = 0) => vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex attribute al (location = 1) => vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// Unbinding VBO, the call to glVertexAttribPointer register the VBO as 
	// the vertex attribute's bound vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// We DON'T unbind the EBO while the VAO is active because the EBO is stored in the VAO
	//glBindVertexArray(GL_ELEMENT_ARRAY_BUFFER, 0);

	// We can unbind the VBO so other calls don't accidentally modify it
	glBindVertexArray(0);


	////////////////////////////////////////
	/* RENDER LOOP */

	Shader shader("Shaders/VertexShader.txt", "Shaders/FragmentShader.txt");

	// Uncomment to draw wireframes
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	double currentTime = glfwGetTime();
	double deltaTime = 0.0f;
	double counter = 0.0f;
	int frameCount = 1;
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		processInput(window);

		// Render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		//float colorValue = ((std::sin(counter += deltaTime * 10.0f) / 4.0f)) + 0.5f;
		//int colorLocation = glGetUniformLocation(shaderProgram, "color");
		//glUniform4f(colorLocation, 0.0f, colorValue, 0.0f, 1.0f);
		
		shader.setInt("iFrame", frameCount);
		glUseProgram(shader.Id);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();

		frameCount++;
	}

	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.Id);

	glfwTerminate();
	return 0;
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
