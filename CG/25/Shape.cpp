#include "Shape.h"

Shape::Shape()
{
	std::cout << "Shape()" << '\n';;
	shapeAngle = 0.f;
	orbitRadius = 0.f;
	orbitSpeed = 0.f;
	rotateY = false;
	rotateX = false;
}

Shape::Shape(float r,float degree)
{
	std::cout << "Shape(float r, float degree)" << '\n';
	shapeAngle = 0.f;
	orbitRadius = r;
	orbitSpeed = 0.f;
	deg.z = degree;
	rotateY = false;
	rotateX = false;
	orbitVertex = nullptr;
}

Shape::Shape(const Shape& other)
{
	this->pos = other.pos;
	this->deg = other.deg;
	this->rot = other.rot;
	this->rotateY = other.rotateY;
	this->rotateX = other.rotateX;
	orbitVertex = nullptr;
}

Shape& Shape::operator=(const Shape& other)
{
	/*if (this != &other) {
		delete shapeVertex;
		pos = other.pos;
		deg = other.deg;
		dir = other.dir;
		rotateY = other.rotateY;
		rotateZ = other.rotateZ;
	}*/

	return *this;
}

Shape::~Shape()
{
	delete shapeVertex;
	delete orbitVertex;
}

void Shape::clearBuffer()
{
	delete shapeVertex;
	delete orbitVertex;
}

void Shape::initVerts(float radius, const Position& color)
{
	std::vector<float> VAO;
	std::vector<unsigned int> VBO;

	const int LATITUDE_SEGMENTS = 20;  // 위도 분할 개수
	const int LONGITUDE_SEGMENTS = 20;  // 경도 분할 개수

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
			VAO.push_back(radius * x);
			VAO.push_back(radius * y);
			VAO.push_back(radius * z);

			if (y == 0.5f) {
				VAO.push_back(1.0f);
				VAO.push_back(1.0f);
				VAO.push_back(1.0f);
			}
			else {
				VAO.push_back(color.x);
				VAO.push_back(color.y);
				VAO.push_back(color.z);
			}
		}
	}

	// 인덱스 데이터 생성 (삼각형들로 구성)
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
}

void  Shape::setRotateY() {
	rotateY = !rotateY;
}

void  Shape::setRotateZ()
{
	rotateX = !rotateX;
}

void Shape::setMoveX(int st)
{
	moveX = st;
}

void Shape::setMoveY(int st)
{
	moveY = st;
}

void Shape::setMoveZ(int st)
{
	moveZ = st;
}

void Shape::setPos(float x, float y, float z)
{
	pos = { x,y,z };
}

void Shape::setRotation(float x, float y, float z)
{
	//rot = { x,y,z };
}

void Shape::setDirection(float x, float y, float z)
{
	rot = { x,y,z };
}

void Shape::setMove(float x, float y, float z)
{
	dir.x = x;
	dir.y = y;
	dir.z = z;
}

void Shape::Reset()
{
	pos = rot = deg = {};
}

void Shape::Update(const Position& center)
{
	if (moveX) {
		if (moveX == 1)
			dir.x += 0.01f;

		else
			dir.x -= 0.01f;
	}

	if (moveY) {
		if (moveY == 1)
			dir.y += 0.01f;

		else
			dir.y -= 0.01f;
	}

	if (moveZ) {
		if (moveZ == 1)
			dir.z += 0.01f;

		else
			dir.z -= 0.01f;
	}

	shapeAngle += orbitSpeed;
	pos.x = orbitRadius * cos(shapeAngle) + dir.x;
	pos.y = dir.y;
	pos.z = orbitRadius * sin(shapeAngle) + dir.z;
	//pos += dir;

	if (rotateY) {
		deg.y += 5.f;
	}

	if (rotateX) {
		deg.x += 5.f;
	}
}

void Shape::Draw(GLuint shaderProgram, const Position& center)
{
	GLuint uLoc = glGetUniformLocation(shaderProgram, "modelTransform");

	if (uLoc < 0)
		std::cout << "uLoc not found" << '\n';

	else {
		glm::mat4 Rx = glm::rotate(glm::mat4(1.f), glm::radians(deg.x), glm::vec3(1.f, 0.f, 0.f));	//	radians(degree) : degree to radian
		glm::mat4 Ry = glm::rotate(glm::mat4(1.f), glm::radians(deg.y), glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 Rz = glm::rotate(glm::mat4(1.f), glm::radians(deg.z), glm::vec3(0.f, 0.f, 1.f));
		glm::mat4 T = glm::translate(glm::mat4(1.f), glm::vec3(pos.x, pos.y, pos.z));
		glm::mat4 S = glm::scale(glm::mat4(1.f), glm::vec3(0.5f));

		glm::mat4 SRT = Rz * Ry * Rx * T * S;
		glUniformMatrix4fv(uLoc, 1, GL_FALSE, glm::value_ptr(SRT));
	}

	glDrawElements(GL_TRIANGLES, shapeVertex->getNumIndices(), GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));
}
