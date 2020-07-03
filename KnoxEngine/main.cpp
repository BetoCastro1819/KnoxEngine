#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "resources/utils/stb_image.h"

#include <iostream>

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	////////////////////////////////////
	//
	// GLFW and GLAD setup
	//
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


	////////////////////////////////////
	//
	// Vertex (and buffers) setup and configuration
	//
	float triangle_1[] = {
		// positions		 	// colors				// texture coords
		 0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 1.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f, 0.0f   // top left
	};

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	unsigned int VAO[1], VBO[1], EBO;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_1), triangle_1, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 3 floats for position + 3 floats for colors + 2 floats for texture coords
	GLsizei vertexStride = sizeof(float) * 8; 
	
	// NOTE Vertex attribute al (location = 0) => vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (void *)0);
	glEnableVertexAttribArray(0);

	// NOTE Vertex attribute al (location = 1) => vertex color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// NOTE Vertex attribute al (location = 2) => texture coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	
	// Unbinding VBO, the call to glVertexAttribPointer register the VBO as 
	// the vertex attribute's bound vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// We DON'T unbind the EBO while the VAO is active because the EBO is stored in the VAO
	//glBindVertexArray(GL_ELEMENT_ARRAY_BUFFER, 0);

	// We can unbind the VBO so other calls don't accidentally modify it
	glBindVertexArray(0);


	////////////////////////////////////
	//
	// LOAD TEXTURE
	//
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 		// Horizontal repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 		// Vertical repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear mipmap interpolation at texture downscale
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear mipmap interpolation at texture upscale

	int width, height, nrChannels;
	const char *textureFilePath = "resources/textures/wood-container.jpg";
	unsigned char *textureData = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);

	if (textureData)
	{
		glTexImage2D(
			GL_TEXTURE_2D, 		// texture target, we bound it to GL_TEXTURE_2D, GL_TEXTURE_1D and GL_TEXTURE_3D will not be affected
			0,					// mipmap level, 0 is the base level, we don't want to create the mipmaps manually
			GL_RGB,				// type of format to store texture, our image only has RGB values, so we set this to GL_RGB
			width, height,		// width and height of the resulting texture
			0, 					// always set this to 0 (some legacy stuff...)
			GL_RGB,				// format of the source image, we stored loaded it with RGB values, so GL_RGB
			GL_UNSIGNED_BYTE,	// datatype of the source image, we stored them as char's (bytes), se we pass GL_UNSIGNED_BYTE
			textureData			// the actual image data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("ERROR: Failed to load texture from %s\n", textureFilePath);
	}

	// NOTE Texture is already loaded to OpenGL, so we can free this memory
	stbi_image_free(textureData);


	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 		// Horizontal repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 		// Vertical repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear mipmap interpolation at texture downscale
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear mipmap interpolation at texture upscale

	textureFilePath = "resources/textures/awesomeface.png";
	textureData = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);

	if (textureData)
	{
		glTexImage2D(
			GL_TEXTURE_2D, 		// texture target, we bound it to GL_TEXTURE_2D, GL_TEXTURE_1D and GL_TEXTURE_3D will not be affected
			0,					// mipmap level, 0 is the base level, we don't want to create the mipmaps manually
			GL_RGB,				// type of format to store texture, our image only has RGB values, so we set this to GL_RGB
			width, height,		// width and height of the resulting texture
			0, 					// always set this to 0 (some legacy stuff...)
			GL_RGBA,			// format of the source image, we stored loaded it with RGB values, so GL_RGBA because .png files have an Alpha value
			GL_UNSIGNED_BYTE,	// datatype of the source image, we stored them as char's (bytes), se we pass GL_UNSIGNED_BYTE
			textureData			// the actual image data
		);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		printf("ERROR: Failed to load texture from %s\n", textureFilePath);
	}

	// NOTE Texture is already loaded to OpenGL, so we can free this memory
	stbi_image_free(textureData);



	////////////////////////////////////////
	//
	// RENDER LOOP
	//
	Shader shader("resources/shaders/VertexShader.txt", "resources/shaders/FragmentShader.txt");

	// NOTE We have to activate the shader before setting uniforms
	glUseProgram(shader.Id);
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

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
		
		shader.setInt("frameCount", frameCount);
		shader.setFloat("mixValue", 0.5f);
		glUseProgram(shader.Id);

		// NOTE GL_TEXTURE0 is activated by default, 
		// so if there is only ONE texture, there is no need to actiavte it manually
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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