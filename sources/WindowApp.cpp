#include "./../includes/WindowApp.hpp"

WindowsApp::WindowsApp() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW." << std::endl;
		exit(1);
	}
	glfwWindowHint(GLFW_SAMPLES, 4); // antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->_window = glfwCreateWindow(WIDTH, HEIGHT, "HumanGL", NULL, NULL);
	if (this->_window == NULL) {
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(this->_window);
	glfwSwapInterval(1);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		exit(1);
	}

	glfwSetInputMode(this->_window, GLFW_STICKY_KEYS, GL_TRUE);

	this->_programID = loadShaders( "./shaders/shader.vert", "./shaders/shader.frag" );

	this->_projection = projectionMat(90.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(this->_window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}

WindowsApp::~WindowsApp() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

bool WindowsApp::isClosed() {
	if (glfwGetKey(this->_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(this->_window) != 0) {
		return (true);
	}
	glfwSwapBuffers(this->_window);
	glfwPollEvents();
	return (false);
}

GLFWwindow *WindowsApp::getWindow() {
	return (this->_window);
}

void WindowsApp::computeMovement() {
	deltaTime();

	static vec3 position{0.0f, 0.0f, 5.0f};
	std::cout << "x : " << position.x << " y : " << position.y << " z : " << position.z << std::endl;
	static float horizontalAngle = M_PI;
	static float verticalAngle = 0.0f;
	static float speed = 3.0f;
	static float cameraSpeed = 1.5f;

	vec3 direction;
	direction.x = cos(0) * sin(M_PI);
	direction.y = sin(0);
	direction.z = cos(0) * cos(M_PI);

	vec3 right;
	right.x = sin(M_PI - M_PI_2);
	right.y = 0;
	right.z = cos(M_PI - M_PI_2);

	vec3 up;
	up = vecCross(right, direction);

	if (glfwGetKey(this->_window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(up, this->_deltaTime), speed));
	}
	if (glfwGetKey(this->_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(up, this->_deltaTime), speed));
	}

	direction.x = cos(0) * sin(horizontalAngle);
	direction.z = cos(0) * cos(horizontalAngle);

	right.x = sin(horizontalAngle - M_PI_2);
	right.z = cos(horizontalAngle - M_PI_2);

	if (glfwGetKey(this->_window, GLFW_KEY_W) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(direction, this->_deltaTime), speed));
	}
	if (glfwGetKey(this->_window, GLFW_KEY_S) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(direction, this->_deltaTime), speed));
	}
	if (glfwGetKey(this->_window, GLFW_KEY_D) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(right, this->_deltaTime), speed));
	}
	if (glfwGetKey(this->_window, GLFW_KEY_A) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(right, this->_deltaTime), speed));
	}
	if (glfwGetKey(this->_window, GLFW_KEY_UP) == GLFW_PRESS) {
		verticalAngle += cameraSpeed * this->_deltaTime;
	}
	if (glfwGetKey(this->_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		verticalAngle -= cameraSpeed * this->_deltaTime;
	}
	if (glfwGetKey(this->_window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		horizontalAngle += cameraSpeed * this->_deltaTime;
	}
	if (glfwGetKey(this->_window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		horizontalAngle -= cameraSpeed * this->_deltaTime;
	}
	if (glfwGetKey(this->_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		speed = 30;
	else
		speed = 3;

	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	right.x = sin(horizontalAngle - M_PI_2);
	right.z = cos(horizontalAngle - M_PI_2);

	up = vecCross(right, direction);

	this->_view = viewMat(position, vecAddition(position, direction), up);
}

GLuint WindowsApp::loadShaders(const char *vertexFilePath, const char *fragmentFilePath) {
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Get the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		std::cout << "Impossible to open " << vertexFilePath << std::endl;
		exit (1);
	}

	// Get the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}else{
		std::cout << "Impossible to open  " << fragmentFilePath << std::endl;
		exit (1);
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}

	// Compile Fragment Shader
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}

	// Link the program
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void WindowsApp::deltaTime(void) {
	static double last = glfwGetTime();

	double now = glfwGetTime();
	this->_deltaTime = (float)(now - last);
	last = now;
}

double WindowsApp::getDeltaTime() {
	return (this->_deltaTime);
}

mat4 WindowsApp::getView() {
	return (this->_view);
}

mat4 WindowsApp::getProjection() {
	return (this->_projection);
}

GLuint WindowsApp::getProgramID() {
	return (this->_programID);
}