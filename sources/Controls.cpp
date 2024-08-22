#include "./../includes/header.h"

extern float deltaTime;

mat4 movement(GLFWwindow *window) {
	static vec3 position{0.0f, 0.0f, 5.0f};
	static float horizontalAngle = 3.14f;
	static float verticalAngle = 0.0f;
	static float speed = 3.0f;
	static float cameraSpeed = 1.5f;

	vec3 direction;
	direction.x = cos(0) * sin(3.14f);
	direction.y = sin(0);
	direction.z = cos(0) * cos(3.14f);

	vec3 right;
	right.x = sin(3.14f - M_PI_2);
	right.y = 0;
	right.z = cos(3.14f - M_PI_2);

	vec3 up;
	up = vecCross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(up, deltaTime), speed));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(up, deltaTime), speed));
	}

	direction.x = cos(0) * sin(horizontalAngle);
	direction.z = cos(0) * cos(horizontalAngle);

	right.x = sin(horizontalAngle - M_PI_2);
	right.z = cos(horizontalAngle - M_PI_2);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(direction, deltaTime), speed));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(direction, deltaTime), speed));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position = vecAddition(position, vecMultiplication(vecMultiplication(right, deltaTime), speed));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position = vecSubstract(position, vecMultiplication(vecMultiplication(right, deltaTime), speed));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		verticalAngle += cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		verticalAngle -= cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		horizontalAngle += cameraSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		horizontalAngle -= cameraSpeed * deltaTime;
	}

	direction.x = cos(verticalAngle) * sin(horizontalAngle);
	direction.y = sin(verticalAngle);
	direction.z = cos(verticalAngle) * cos(horizontalAngle);

	right.x = sin(horizontalAngle - M_PI_2);
	right.z = cos(horizontalAngle - M_PI_2);

	up = vecCross(right, direction);

	mat4 view = viewMat(position, vecAddition(position, direction), up);

	return (view);
}