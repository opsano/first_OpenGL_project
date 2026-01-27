#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main() {
	// initalize glfw & configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window object (glfw)
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// init viewport
	glViewport(0, 0, 800, 600);

	// resize viewport if window gets resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Nomralized device coordinates for a triangle
	float verticies[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
	};

	unsigned int VBO; // vertex buffer object
	glGenBuffers(1, &VBO); // generate buffers & assigning a ID to our VBO object. Buffers are made behind the scenes & are accessed through our object ID's generated here
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // left argument is a buffer type, right argument is our cooresponding buffer ID. Binds id to cooresponding buffer type (use of GL_ARRAY_BUFFER will configure VBO)



	// render loop - put all rendering commands here
	while (!glfwWindowShouldClose(window)) {
		// input: checks for escape key, if so closes window
		processInput(window);

		glClearColor(0.1, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, window resized, etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// terminate and free all allocated resources
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

// whenever the window size is changed, this executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// makes sure that the viewport matches the new window dimensions;
	glViewport(0, 0, width, height);
}