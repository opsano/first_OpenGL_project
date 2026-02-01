#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// prototypes
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// vertex shader source code written in GLSL
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// fragment shader source code written in GLSL
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

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
	// resize viewport if window gets resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}



	// vertex Shader
	// -------------------------
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // creates shader object behind the scenes & assigns its ID to our vertexShader 

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // attach the shader source code to the shader object
	glCompileShader(vertexShader); // compile shader

	// check for compile-time errors with our vertex shader
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	// if compilation fails, we wanna retrieve error messages
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	// -------------------------
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for compile-time errors with fragment shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders
	// -------------------------
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); // create shader program & assign the ID reference
	glAttachShader(shaderProgram, vertexShader); // attach vertexShader to the program
	glAttachShader(shaderProgram, fragmentShader); // attach fragmentShader to the program
	glLinkProgram(shaderProgram); // link the two
	// check status of link
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) { // if link failed, get info on it
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	// delete the shaders, since they are linked to the program, and we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Normalized device coordinates for a square
	float verticies[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	
	// creating two triangles from our points to create square
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3 // second triangle

	};

	unsigned int VAO, VBO, EBO; // vertex array object, vertex buffer object, and element buffer object
	glGenVertexArrays(1, &VAO); // generate our id
	glGenBuffers(1, &VBO); // generate buffers & assigning a ID to our VBO object. Buffers are made behind the scenes & are accessed through our object ID's generated here
	glGenBuffers(1, &EBO); // generate EBO behind scenes	

	glBindVertexArray(VAO); // bind VAO first, then bind and set vertex buffer(s), and then configure vertex attribute(s)

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // left argument is a buffer type, right argument is our cooresponding buffer ID. Binds id to cooresponding buffer type (use of GL_ARRAY_BUFFER will configure VBO)
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW); // copies verticies into the buffer's memory, used to copy user-defined data into the currently bound buffer (which is VBO rn)

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // tells OpenGL how to interpret vertex data in the form of a vertex attribute
	glEnableVertexAttribArray(0); // enables a vertex attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// render loop - put all rendering commands here
	while (!glfwWindowShouldClose(window)) {
		// input: checks for escape key, if so closes window
		processInput(window);


		glClearColor(0.1, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram); // activate the program. Every shader and rendering call after this point will use this program object (and thus the shaders)
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfw: swap buffers and poll IO events (keys pressed/released, mouse moved, window resized, etc)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// terminate and free all allocated resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
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