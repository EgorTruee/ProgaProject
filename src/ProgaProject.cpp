#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Shader.h"
#include "ShaderProgram.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera.h"

float U(float ro)
{
	return -10 / ro;
}
float F(float ro, float E0, float L0, float m, float sgn)
{
	float Val = E0 - L0 * L0 / (2 * m * ro * ro) - U(ro);
	static float ValP = Val;
	static float sgnt = -1;

	if (ValP * Val < 0)
	{
		sgnt *= -1;
	}

	return sgnt * sqrt(abs(E0 - L0 * L0 / (2 * m * ro * ro) - U(ro)));
}
std::vector<std::pair<float, float>> Solve(float m, float phi0, float phiDot0, float ro0, float roDot0, float dt, int n)
{
	std::vector<std::pair<float, float>> Res(n);
	float E0 = m * (roDot0 * roDot0 + ro0 * phiDot0 * ro0 * phiDot0) / 2 + U(ro0);
	float L0 = m * ro0 * ro0 * phiDot0;
	float sgn = (signbit(roDot0) ? -1 : 1);
	float sgnp = sgn;

	Res[0] = { ro0, phi0 };

	for (int i = 1; i < n; i++)
	{
		float w1 = F(Res[i - 1].first, E0, L0, m, sgn);
		float w2 = F(Res[i - 1].first + 2.f / 3.f * w1 * dt, E0, L0, m, sgn);
		Res[i].first = Res[i - 1].first + w1 * dt / 4.f + 3.f / 4.f * w2 * dt;
		Res[i].second = Res[i - 1].second + dt * (signbit(phiDot0) ? -1 : 1) * L0 / m / Res[i].first / Res[i].first;
		sgnp = (signbit(2 * F(Res[i].first, E0, L0, m, sgn) - F(Res[i - 1].first, E0, L0, m, sgnp)) ? -1 : 1);

		std::swap(sgn, sgnp);
	}
	return Res;
}
std::vector<float> GetPoints()
{
 	std::vector<std::pair<float, float>> Solution = Solve(1, 0, 0.5, 2, -3, 0.001, 10000);
	std::vector<float> Res(Solution.size() * 3);

	for (int i = 0; i < Solution.size(); i++)
	{
		Res[i * 3] = Solution[i].first * sin(Solution[i].second);
		Res[i * 3 + 1] = 0;
		Res[i * 3 + 2] = Solution[i].first * cos(Solution[i].second);
	}
	return Res;
}
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLint WindowWidth = 1000;
	GLint WindowHeight = 1000;
	GLFWwindow* Window = glfwCreateWindow(WindowWidth, WindowHeight, "Simulation", nullptr, nullptr);

	if (Window == nullptr)
	{
		std::cerr << "Window creation failed" << std::endl;

		glfwTerminate();
		
		return -1;
	}
	glfwMakeContextCurrent(Window);
	glfwSetKeyCallback(Window, KeyboardCollback);
	glfwSetCursorPosCallback(Window, MouseCallback);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Glew initalization failed" << std::endl;

		glfwTerminate();

		return -2;
	}
	int Width;
	int Height;

	glfwGetFramebufferSize(Window, &Width, &Height);
	glViewport(0, 0, Width, Height);

	ShaderProgram Program;

	try
	{
		Shader Vertex(GL_VERTEX_SHADER, "D:/Projects/ProgaProject1/Shaders/Vertex.glsl");
		Shader Fragment(GL_FRAGMENT_SHADER, "D:/Projects/ProgaProject1/Shaders/Fragment.glsl");

		Program.AttachShader(Vertex);
		Program.AttachShader(Fragment);
		Program.Link();
		Program.DetachShader(Vertex);
		Program.DetachShader(Fragment);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
	GLuint VAO;
	GLuint VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	std::vector<float> Vertices = GetPoints();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	glBindVertexArray(0);

	GLuint AxisVAO;
	GLuint AxisVBO;

	glGenVertexArrays(1, &AxisVAO);
	glGenBuffers(1, &AxisVBO);

	glBindVertexArray(AxisVAO);

	float AxisVertices[] = {
		0.f, 0.f, 0.f, 10.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f, 10.f, 0.f,
		0.f, 0.f, 0.f, 0.f, 0.f, 10.f
	};
	glBindBuffer(GL_ARRAY_BUFFER, AxisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(AxisVertices), AxisVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);

	glBindVertexArray(0);


	Program.Use();

	Camera Cam; 
	float OldTime = glfwGetTime();
	float NewTime = OldTime;
	glm::mat4 Projection = glm::mat4(1.f);
	glm::mat4 Model = glm::mat4(1.f);

	Projection = glm::perspective(glm::radians(90.f), static_cast<float>(Width) / static_cast<float>(Height), 0.1f, 100.f);

	while (!glfwWindowShouldClose(Window))
	{
		OldTime = NewTime;
		NewTime = glfwGetTime();

		Cam.Update(OldTime - NewTime);

		glfwPollEvents();

		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		Program.Uniform("Model", Model);
		Program.Uniform("View", Cam.GetViewTransform());
		Program.Uniform("Projection", Projection);
		Program.Uniform("Color", 1.0, 0.0, 0.0, 1.0);

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 10000);

		Program.Uniform("Color", 0.0, 1.0, 0.0, 1.0);

		glBindVertexArray(AxisVAO);
		glDrawArrays(GL_LINES, 0, 6);

		glBindVertexArray(0);

		glfwSwapBuffers(Window);
	}
	glfwTerminate();

	return 0;
}
