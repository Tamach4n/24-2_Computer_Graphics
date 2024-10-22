#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';
	xPos = yPos = 0.f;
	xDeg = 30.f;
	yDeg = -30.f;
	xRot = yRot = 0.f;
	xDir = yDir = 0.f;
}

Shape::Shape(float x, float y)
	:xPos(x), yPos(y)
{
	std::cout << "Shape(float x, float y)" << '\n';
	xDeg = 30.f;
	yDeg = -30.f;
	xRot = yRot = 0.f;
	xDir = yDir = 0.f;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
}

void Shape::InitCubeVerts()
{	
	float VAO[] = {
		-0.5f, -0.5f, -0.5f,   0.1f, 0.1f, 0.1f,
		 0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 1.0f
	};

	unsigned int VBO[] = {
		// 앞면
		4, 5, 6, 6, 7, 4,
		// 뒷면
		0, 1, 2, 2, 3, 0,
		// 왼쪽 면
		0, 3, 7, 7, 4, 0,
		// 오른쪽 면
		1, 5, 6, 6, 2, 1,
		// 아래쪽 면
		0, 1, 5, 5, 4, 0,
		// 위쪽 면
		3, 2, 6, 6, 7, 3
	};

	shapeVertex = new Vertex(VAO, 8, VBO, 36);
}

void Shape::InitConeVerts()
{
	int numSegments = 36;

	std::vector<float> VAO;
	std::vector<unsigned int> VBO;

	// 꼭짓점 - 0
	VAO.push_back(0.0f);
	VAO.push_back(0.5f);
	VAO.push_back(0.0f);
	VAO.push_back(0.0f);
	VAO.push_back(1.0f);
	VAO.push_back(0.0f);

	// 밑변 중심점 - 1
	VAO.push_back(0.0f);
	VAO.push_back(-0.5f);
	VAO.push_back(0.0f);
	VAO.push_back(0.0f);
	VAO.push_back(1.0f);
	VAO.push_back(0.0f);

	// 밑면의 정점들 - 2 ~ numSegments+1
	float angleStep = 2.0f * M_PI / numSegments;
	float theta, x, z;

	for (int i = 0; i <= numSegments; ++i) {
		theta = i * angleStep;
		x = 0.5f * cos(theta);
		z = 0.5f * sin(theta);

		VAO.push_back(x);	
		VAO.push_back(-0.5f);
		VAO.push_back(z);	
		VAO.push_back(0.f);	
		VAO.push_back(1.f);	
		VAO.push_back(0.f);	
	}
	
	int centerNum = VAO.size() - 1;

	for (int i = 2; i < numSegments + 2; ++i) {
		VBO.push_back(0);
		VBO.push_back(i);
		VBO.push_back(i + 1);
	}

	for (int i = 2; i < numSegments + 2; ++i) {
		VBO.push_back(1);
		VBO.push_back(i);
		VBO.push_back(i + 1);
	}

	shapeVertex = new Vertex(VAO, VBO);
}

void Shape::InitSphereVerts()
{
	const int LATITUDE_SEGMENTS = 20;  // 위도 분할 개수
	const int LONGITUDE_SEGMENTS = 20;  // 경도 분할 개수

	std::vector<float> VAO;
	std::vector<unsigned int> VBO;

	// 정점 데이터 생성 (위도와 경도를 따라 분할)
	for (int i = 0; i <= LATITUDE_SEGMENTS; ++i) {
		float theta = i * M_PI / LATITUDE_SEGMENTS;  // 위도 각도
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int j = 0; j <= LONGITUDE_SEGMENTS; ++j) {
			float phi = j * 2.0f * M_PI / LONGITUDE_SEGMENTS;  // 경도 각도
			float x = cos(phi) * sinTheta;
			float y = cosTheta;
			float z = sin(phi) * sinTheta;

			// 정점 (x, y, z)
			VAO.push_back(0.5f * x);
			VAO.push_back(0.5f * y);
			VAO.push_back(0.5f * z);
			VAO.push_back(0.5f);
			VAO.push_back(0.5f);
			VAO.push_back(0.5f);
		}
	}

	// 인덱스 데이터 생성
	for (int i = 0; i < LATITUDE_SEGMENTS; ++i) {
		for (int j = 0; j < LONGITUDE_SEGMENTS; ++j) {
			int first = (i * (LONGITUDE_SEGMENTS + 1)) + j;
			int second = first + LONGITUDE_SEGMENTS + 1;

			// 삼각형 1
			VBO.push_back(first);
			VBO.push_back(second);
			VBO.push_back(first + 1);

			// 삼각형 2
			VBO.push_back(second);
			VBO.push_back(second + 1);
			VBO.push_back(first + 1);
		}
	}

	shapeVertex = new Vertex(VAO, VBO);
}

void Shape::setActive(Shader* shader)
{
	shapeVertex->setActive();

	GLuint uLoc = glGetUniformLocation(shader->GetshaderProgram(), "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(xDeg), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(yDeg), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Tx = glm::translate(glm::mat4(1.f), glm::vec3(xPos, 0.f, 0.f));
		glm::mat4 Ty = glm::translate(glm::mat4(1.f), glm::vec3(0.f, yPos, 0.f));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRT = Ty * Tx * Ry * Rx * S;

		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));
	}
}

void Shape::setRotation(float x, float y)
{
	xRot = x;
	yRot = y;
}

void Shape::setDirection(float x, float y)
{
	xDir = x;
	yDir = y;
}

void Shape::Reset()
{
	xPos = yPos = xRot = yRot = xDir = yDir = 0.f;
	//xDeg = 30.f;
	//yDeg = -30.f;
}

void Shape::Update()
{
	xPos += xDir;
	yPos += yDir;
	xDeg += xRot;
	//xDeg += 1.f;
	yDeg += yRot;
	//yDeg += 1.f;
}

void Shape::Draw()
{
	glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
