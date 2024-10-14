#include <iostream>
#include <fstream>
#include <chrono>
#include "Scene.h"

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	selectMode = 1;
}

void Scene::initialize()
{
	sceneShader = makeShader("C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-9\\vertex.glsl", "C:\\Users\\worl\\Desktop\\Lecture\\2-2\\CG\\CG\\1-9\\fragment.glsl");

	initBuffer();
}

void Scene::update()
{
	for (auto& t : triangles) {
		t.update(selectMode);
	}
}

void Scene::draw()
{
	glUseProgram(sceneShader);

	for (auto& t : triangles) {
		t.draw(sceneShader, VAO);
	}
}

void Scene::keyboard(unsigned char key)
{
	switch (key) {
	case '1':
		selectMode = 1;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '2':
		selectMode = 2;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '3':
		selectMode = 3;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;

	case '4':
		selectMode = 4;

		for (int i = 0; i < 4; ++i)
			triangles[i].setMode(selectMode);

		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
}

void Scene::specialKeyboardUp(int key)
{
}

void Scene::mouse(int button, int state, int x, int y)
{
	// 화면 업데이트가 된다....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			std::cout << "좌클릭 : " << x << ", " << y << std::endl;
			std::cout << "OpenGL x 좌표는 " << (static_cast<float>(x) / width * 2.f - 1.f) << std::endl;
			std::cout << "OpenGL y 좌표는 " << (static_cast<float>(height - y) / height * 2.f - 1.f) << std::endl;
			break;

		case GLUT_MIDDLE_BUTTON:
			std::cout << "휠클릭 : " << x << ", " << y << std::endl;
			break;

		case GLUT_RIGHT_BUTTON:
			std::cout << "우클릭 : " << x << ", " << y << std::endl;
			break;

		case WHEEL_UP:
			std::cout << "휠  업 : " << x << ", " << y << std::endl;
			break;

		case WHEEL_DOWN:
			std::cout << "휠다운 : " << x << ", " << y << std::endl;
			break;
		}

		break;

	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			break;

		case GLUT_RIGHT_BUTTON:
			break;

		case GLUT_MIDDLE_BUTTON:
			break;
		}

		break;
	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

void Scene::initBuffer()
{
	glGenVertexArrays(4, VAO);

	float vertexArray[4][15]
	{
		{	// 위
			0.0f, 0.2f,		1.f, 0.f, 0.f,	// 1번점 (정점(x, y), 색상(r, g, b))
			-0.1f, -0.2f,	0.f, 1.f, 0.f,	// 2번점 (정점(x, y), 색상(r, g, b))
			0.1f, -0.2f,	0.f, 0.f, 1.f,	// 3번점 (정점(x, y), 색상(r, g, b))
		},
		{	// 아래
			0.0f, -0.2f,	1.f, 0.f, 0.f,	// 1번점 (정점(x, y), 색상(r, g, b))
			0.1f, 0.2f,		0.f, 1.f, 0.f,	// 2번점 (정점(x, y), 색상(r, g, b))
			-0.1f, 0.2f,	0.f, 0.f, 1.f,	// 3번점 (정점(x, y), 색상(r, g, b))
		},
		{	// 왼쪽
			-0.2f, 0.0f,	1.f, 0.f, 0.f,	// 1번점 (정점(x, y), 색상(r, g, b))
			0.2f, -0.1f,	0.f, 1.f, 0.f,	// 2번점 (정점(x, y), 색상(r, g, b))
			0.2f, 0.1f,		0.f, 0.f, 1.f,	// 3번점 (정점(x, y), 색상(r, g, b))
		},
		{	// 오른쪽
			0.2f, 0.0f,		1.f, 0.f, 0.f,	// 1번점 (정점(x, y), 색상(r, g, b))
			-0.2f, 0.1f,	0.f, 1.f, 0.f,	// 2번점 (정점(x, y), 색상(r, g, b))
			-0.2f, -0.1f,	0.f, 0.f, 1.f,	// 3번점 (정점(x, y), 색상(r, g, b))
		}
	};

	for (int i = 0; i < 4; ++i) {
		glBindVertexArray(VAO[i]);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// CPU 메모리에 있는 데이터를, GPU 메모리에 복사
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, &vertexArray[i], GL_STATIC_DRAW);

		// 이 데이터가 어떤 데이터인지, 우리가 정의를 했기 때문에, openGL에 알려줘야 한다!
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		// location, 갯수, 타입, normalized?, 간격(바이트), 시작오프셋
		glEnableVertexAttribArray(0);

		// 이 데이터가 어떤 데이터인지, 우리가 정의를 했기 때문에, openGL에 알려줘야 한다!
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, reinterpret_cast<void*>(sizeof(float) * 2));
		glEnableVertexAttribArray(1);
	}
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// 쉐이더 만들자...

	auto m_start = std::chrono::system_clock::now();

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// 이 변수는 지역변수라서
	const char* vertexSource{ vertexStr.c_str() };		// 이 변수에 저장되려면 위 변수가 소멸되면 안된다..!

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << vertexFilename << " ERROR: vertex shader error\n" << errorLog << std::endl;
	}

	else {
		std::cout << vertexFilename << " 컴파일 성공!" << std::endl;
	}

	std::string fragmentStr = readFile(fragmentFilename);
	const char* fragmentSource{ fragmentStr.c_str() };

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << fragmentFilename << " ERROR: fragment shader error\n" << errorLog << std::endl;
	}

	else {
		std::cout << fragmentFilename << " 컴파일 성공!" << std::endl;
	}

	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);		// 만든 vertex Shader를 쉐이더 프로그램에 추가해 준다
	glAttachShader(shaderID, fragmentShader);	// 만든 fragment Shader를 쉐이더 프로그램에 추가해 준다
	glLinkProgram(shaderID);					// 추가한 쉐이더들을 하나의 프로그램으로 생성한다!

	glDeleteShader(vertexShader);				// 이제 더이상 vertex Shaer는 필요없다...
	glDeleteShader(fragmentShader);				// 이제 더이상 fragment Shaer는 필요없다...

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		//return;
	}

	else {
		std::cout << vertexFilename << ", " << fragmentFilename << " shader program 생성 성공!" << std::endl;
	}
	
	auto m_end = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli>dur = m_end - m_start;
	std::cout << dur.count() << "ms" << '\n';

	return shaderID;
}

std::string Scene::readFile(std::string filename)
{
	std::ifstream in{ filename };

	if (!in) {
		std::cout << filename << "파일 존재하지 않음!" << std::endl;
		exit(1);
	}

	std::string str;
	std::string temp;

	while (std::getline(in, temp)) {
		str += temp;
		str.append(1, '\n');
	}

	return str;
}
